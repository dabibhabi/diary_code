#pragma once

#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/domain/email.hpp"
#include "cuny_sms/domain/employee_id.hpp"
#include "cuny_sms/domain/person.hpp"

namespace cuny_sms {

// Pure, deterministic email derivation. Format:
//
//   {campusCode}_{firstInitial}{lastName}{lastNDigitsEmpId}@{emailDomain}
//
// Last-3 digits chosen by the birthday-paradox argument in
// docs/math/13-probability.md (last-2 hits 50% collision at k≈12, which is
// not enough for any common name). Adjustable via the `suffixDigits` param.
class EmailGenerator {
public:
    static constexpr std::size_t kDefaultSuffixDigits = 3;

    [[nodiscard]] static Email generate(CampusCode campus, const Person& person, EmployeeId emplid,
                                        std::size_t suffixDigits = kDefaultSuffixDigits) {
        std::string local;
        local.reserve(32);

        // {campusCode}_
        local.append(toString(campus));
        local.push_back('_');

        // {firstInitial}
        local.push_back(
            static_cast<char>(std::tolower(static_cast<unsigned char>(person.first().front()))));

        // {lastName}
        local.append(toLower(person.last()));

        // {lastNDigitsEmpId}
        local.append(emplid.lastDigits(suffixDigits));

        // @{emailDomain}
        local.push_back('@');
        local.append(emailDomainFor(campus));

        return Email{std::move(local)};
    }

private:
    [[nodiscard]] static std::string toLower(std::string_view s) {
        std::string out;
        out.reserve(s.size());
        for (char c : s) {
            out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
        return out;
    }
};

}  // namespace cuny_sms
