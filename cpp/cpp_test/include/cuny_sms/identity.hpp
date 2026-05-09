#pragma once

// Umbrella header for the Phase 1 identity + factory types. Including this
// gets you Name, Person, EmployeeId, Email, EmailGenerator, Campus(Code),
// CunyMember, the (lightweight) Role hierarchy, Course types, and the two
// abstract factory hierarchies (StaffFactory, CourseFactory).

#include "cuny_sms/factories/admin_factory.hpp"
#include "cuny_sms/domain/admin_level.hpp"
#include "cuny_sms/domain/campus.hpp"
#include "cuny_sms/domain/campus_code.hpp"
#include "cuny_sms/factories/campus_services.hpp"
#include "cuny_sms/persistence/catalog_loader.hpp"
#include "cuny_sms/domain/course.hpp"
#include "cuny_sms/factories/course_factory.hpp"
#include "cuny_sms/domain/course_id.hpp"
#include "cuny_sms/domain/cuny_member.hpp"
#include "cuny_sms/domain/department.hpp"
#include "cuny_sms/domain/email.hpp"
#include "cuny_sms/domain/email_generator.hpp"
#include "cuny_sms/domain/employee_id.hpp"
#include "cuny_sms/factories/faculty_factory.hpp"
#include "cuny_sms/domain/meeting_slot.hpp"
#include "cuny_sms/domain/name.hpp"
#include "cuny_sms/domain/person.hpp"
#include "cuny_sms/graph/prerequisite_graph.hpp"
#include "cuny_sms/domain/role.hpp"
#include "cuny_sms/domain/section.hpp"
#include "cuny_sms/factories/section_factory.hpp"
#include "cuny_sms/factories/student_factory.hpp"
#include "cuny_sms/factories/teaching_assistant_factory.hpp"
