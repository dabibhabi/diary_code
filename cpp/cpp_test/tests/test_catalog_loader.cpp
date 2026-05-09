#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <stdexcept>

#include "cuny_sms/persistence/catalog_loader.hpp"

using namespace cuny_sms;  // NOLINT(google-build-using-namespace)

namespace fs = std::filesystem;

// Set by CMake at configure time to the absolute data/catalogs/ directory.
#ifndef CUNY_CATALOG_DIR
#error "CUNY_CATALOG_DIR must be defined; see tests/CMakeLists.txt"
#endif

namespace {

const fs::path catalogDir{CUNY_CATALOG_DIR};

struct ExpectedFile {
    CampusCode  campus;
    Department  dept;
    int         minRows;   // lower bound; agent reported counts may grow
};

constexpr int kAnyPositive = 1;

}  // namespace

TEST_CASE("CatalogLoader path convention is {campus}_{dept}.tsv",
          "[catalog][loader]") {
    REQUIRE(CatalogLoader::pathFor(CampusCode::CCNY,     Department::Math)    == "ccny_math.tsv");
    REQUIRE(CatalogLoader::pathFor(CampusCode::Brooklyn, Department::CS)      == "brooklyn_cs.tsv");
    REQUIRE(CatalogLoader::pathFor(CampusCode::Hunter,   Department::Stats)   == "hunter_stats.tsv");
    REQUIRE(CatalogLoader::pathFor(CampusCode::Hunter,   Department::Physics) == "hunter_physics.tsv");
}

TEST_CASE("CatalogLoader reads every (campus, dept) file in data/catalogs/",
          "[catalog][loader][integration]") {
    const std::vector<ExpectedFile> expected{
        // campus,             dept,                 minRows
        {CampusCode::CCNY,     Department::Math,     50},
        {CampusCode::CCNY,     Department::CS,       kAnyPositive},  // PARTIAL per README
        {CampusCode::CCNY,     Department::Stats,    5},
        {CampusCode::CCNY,     Department::Physics,  kAnyPositive},  // PARTIAL per README
        {CampusCode::Brooklyn, Department::Math,     50},
        {CampusCode::Brooklyn, Department::CS,       50},
        {CampusCode::Brooklyn, Department::Stats,    5},
        {CampusCode::Brooklyn, Department::Physics,  20},
        {CampusCode::Hunter,   Department::Math,     30},
        {CampusCode::Hunter,   Department::CS,       20},
        {CampusCode::Hunter,   Department::Stats,    5},
        {CampusCode::Hunter,   Department::Physics,  20},
    };

    std::size_t totalCourses = 0;
    for (const auto& e : expected) {
        const auto entries = CatalogLoader::loadCatalog(catalogDir, e.campus, e.dept);
        INFO("file: " << CatalogLoader::pathFor(e.campus, e.dept).string()
             << "  rows: " << entries.size());
        REQUIRE(static_cast<int>(entries.size()) >= e.minRows);

        // Every entry has a non-empty id and a level in [100, 499].
        for (const auto& ent : entries) {
            REQUIRE_FALSE(ent.courseId.empty());
            REQUIRE(ent.level >= 100);
            REQUIRE(ent.level <= 499);
            REQUIRE_FALSE(ent.title.empty());
            // credits is optional; either nullopt or a small positive integer.
            if (ent.credits.has_value()) {
                REQUIRE(*ent.credits >= 0);
                REQUIRE(*ent.credits <= 12);
            }
        }
        totalCourses += entries.size();
    }
    INFO("total courses across all 12 files: " << totalCourses);
    REQUIRE(totalCourses >= 400);
}

TEST_CASE("CatalogLoader throws on missing file", "[catalog][loader]") {
    REQUIRE_THROWS_AS(CatalogLoader::loadFile("/nonexistent/path.tsv"),
                      std::runtime_error);
}
