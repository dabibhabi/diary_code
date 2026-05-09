#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/department.hpp"

namespace cuny_sms {

// One row out of a data/catalogs/{campus}_{dept}.tsv file. The
// course_id is the campus's local code (e.g., "MATH 20100", "CSCI 127");
// `credits` is nullopt when the upstream catalog page omits it.
struct CatalogEntry {
    std::string        courseId;
    int                level{0};
    std::string        title;
    std::optional<int> credits;
};

// Reads the TSV catalog files produced by the data-collection step.
//
// Each file:
//   - lines starting with `#` are comment metadata (source URLs, notes)
//   - the first non-comment line is the header `course_id\tlevel\ttitle\tcredits`
//   - subsequent lines are data rows with the same 4 tab-separated fields
//   - `credits` may be `?` to signal "unknown"
class CatalogLoader {
public:
    [[nodiscard]] static std::vector<CatalogEntry>
    loadFile(const std::filesystem::path& path) {
        std::ifstream in{path};
        if (!in) {
            throw std::runtime_error("CatalogLoader: cannot open " + path.string());
        }

        std::vector<CatalogEntry> out;
        std::string               line;
        bool                      headerSeen = false;
        std::size_t               lineNo     = 0;

        while (std::getline(in, line)) {
            ++lineNo;
            if (line.empty()) continue;
            if (line.front() == '#') continue;

            if (!headerSeen) {
                // First non-comment line must be the header.
                if (line.rfind("course_id", 0) != 0) {
                    throw std::runtime_error(
                        path.string() + ":" + std::to_string(lineNo) +
                        ": expected header row, got: " + line);
                }
                headerSeen = true;
                continue;
            }

            out.push_back(parseRow(line, path, lineNo));
        }
        return out;
    }

    // Convenience: build the conventional path {baseDir}/{campus}_{dept}.tsv.
    [[nodiscard]] static std::vector<CatalogEntry>
    loadCatalog(const std::filesystem::path& baseDir,
                CampusCode campus, Department dept) {
        return loadFile(baseDir / pathFor(campus, dept));
    }

    [[nodiscard]] static std::filesystem::path
    pathFor(CampusCode campus, Department dept) {
        std::string fname;
        fname.append(toString(campus));
        fname.push_back('_');
        fname.append(deptSlug(dept));
        fname.append(".tsv");
        return fname;
    }

private:
    [[nodiscard]] static std::string_view deptSlug(Department d) noexcept {
        switch (d) {
            case Department::Math:    return "math";
            case Department::CS:      return "cs";
            case Department::Stats:   return "stats";
            case Department::Physics: return "physics";
        }
        return "unknown";
    }

    [[nodiscard]] static CatalogEntry
    parseRow(std::string_view line, const std::filesystem::path& path, std::size_t lineNo) {
        std::vector<std::string_view> fields;
        std::size_t                   start = 0;
        for (std::size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '\t') {
                fields.emplace_back(line.substr(start, i - start));
                start = i + 1;
            }
        }
        fields.emplace_back(line.substr(start));

        if (fields.size() != 4) {
            std::ostringstream os;
            os << path.string() << ":" << lineNo << ": expected 4 tab-separated fields, got "
               << fields.size();
            throw std::runtime_error(os.str());
        }

        CatalogEntry e;
        e.courseId.assign(fields[0]);
        e.title.assign(fields[2]);

        try {
            e.level = std::stoi(std::string{fields[1]});
        } catch (const std::exception&) {
            throw std::runtime_error(
                path.string() + ":" + std::to_string(lineNo) +
                ": bad level value: " + std::string{fields[1]});
        }

        if (fields[3] != "?" && !fields[3].empty()) {
            try {
                e.credits = std::stoi(std::string{fields[3]});
            } catch (const std::exception&) {
                throw std::runtime_error(
                    path.string() + ":" + std::to_string(lineNo) +
                    ": bad credits value: " + std::string{fields[3]});
            }
        }

        return e;
    }
};

}  // namespace cuny_sms
