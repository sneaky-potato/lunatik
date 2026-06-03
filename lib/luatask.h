/*
* SPDX-FileCopyrightText: (c) 2026 Ashwani Kumar Kamal <ashwanikamal.im421@gmail.com>
* SPDX-License-Identifier: MIT OR GPL-2.0-only
*/

#ifndef luatask_h
#define luatask_h

#include <lunatik.h>

typedef struct {
	struct task_struct *task;
} luatask_t;

lunatik_object_t *luatask_new(lua_State *L, struct task_struct *task);

#endif

