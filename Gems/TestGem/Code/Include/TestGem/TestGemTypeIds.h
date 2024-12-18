
#pragma once

namespace TestGem
{
    // System Component TypeIds
    inline constexpr const char* TestGemSystemComponentTypeId = "{5F23846C-EA49-4215-8B05-9F6EE2282E49}";
    inline constexpr const char* TestGemEditorSystemComponentTypeId = "{4E39302E-E163-4FEE-B397-73FB224BFDED}";

    // Module derived classes TypeIds
    inline constexpr const char* TestGemModuleInterfaceTypeId = "{30F51C9E-4D69-41EB-ACC9-BB931DD59792}";
    inline constexpr const char* TestGemModuleTypeId = "{E3A3670D-9676-425E-BB75-DBB750EC61BF}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* TestGemEditorModuleTypeId = TestGemModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* TestGemRequestsTypeId = "{62304EC9-2B67-4931-B831-7EC771CED7DC}";
} // namespace TestGem
