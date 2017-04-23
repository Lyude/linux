/*
 * Copyright 2017 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: Lyude Paul
 */
#include "priv.h"

enum gf100_clkgate_ctrl {
	GF100_CLKGATE_CTRL_PGRAPH = 0,
	GF100_CLKGATE_CTRL_PPDEC  = 1,
	GF100_CLKGATE_CTRL_PPPP   = 2,
	GF100_CLKGATE_CTRL_PVLD   = 3,
	GF100_CLKGATE_CTRL_PCOPY  = 4,
	GF100_CLKGATE_CTRL_PCOPY1 = 5,
	GF100_CLKGATE_CTRL_PVENC  = 6,
	GF100_CLKGATE_CTRL_PCOPY2 = 7,
};

static int
gf100_clkgate_init(struct nvkm_therm *therm, enum nvkm_therm_clkgate_mode mode)
{
	int i;

	for (i = GF100_CLKGATE_CTRL_PGRAPH; i <= GF100_CLKGATE_CTRL_PCOPY2; i++)
		nvkm_wr32(therm->subdev.device, 0x20200 + (i * 4), mode);

	return 0;
}

static const struct nvkm_therm_func
gf100_therm = {
	.init = gt215_therm_init,
	.fini = g84_therm_fini,
	.pwm_ctrl = nv50_fan_pwm_ctrl,
	.pwm_get = nv50_fan_pwm_get,
	.pwm_set = nv50_fan_pwm_set,
	.pwm_clock = nv50_fan_pwm_clock,
	.temp_get = g84_temp_get,
	.fan_sense = gt215_therm_fan_sense,
	.program_alarms = nvkm_therm_program_alarms_polling,
	.clkgate_init = gf100_clkgate_init,
};

int
gf100_therm_new(struct nvkm_device *device, int index,
		struct nvkm_therm **ptherm)
{
	return nvkm_therm_new_(&gf100_therm, device, index, ptherm);
}
