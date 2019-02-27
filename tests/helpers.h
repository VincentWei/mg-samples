/*
** helpers.h:
**  Helpers for test code of MiniGUI 3.4.0
**
** Copyright (C) 2019 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef _MG_TESTS_HELPERS
    #define _MG_TESTS_HELPERS

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int uc32_to_utf8(Uchar32 c, char* outbuf);
const char* get_general_category_name(UCharGeneralCategory gc);
const char* get_break_type_name(UCharBreakType bt);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _MG_TESTS_HELPERS */

