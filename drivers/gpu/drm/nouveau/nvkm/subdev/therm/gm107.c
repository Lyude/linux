/*
 * Copyright 2014 Martin Peres
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
 * Authors: Martin Peres
 */
#include "priv.h"
#include "gk104.h"

void
gm107_clkgate_enable(struct nvkm_therm *base)
{
	struct gk104_therm *therm = gk104_therm(base);
	struct nvkm_device *dev = therm->base.subdev.device;
	const struct gk104_clkgate_engine_info *order = therm->clkgate_order;
	int i;

	/* Program ENG_MANT, ENG_FILTER */
	for (i = 0; order[i].engine != NVKM_SUBDEV_NR; i++) {
		if (!nvkm_device_subdev(dev, order[i].engine))
			continue;

		nvkm_mask(dev, 0x20200 + order[i].offset, 0xff00, 0x2200);
	}

	/* magic */
	nvkm_wr32(dev, 0x020288, therm->idle_filter->fecs);
	nvkm_wr32(dev, 0x02028c, therm->idle_filter->hubmmu);

	/* Enable clockgating (ENG_CLK = RUN->AUTO) */
	for (i = 0; order[i].engine != NVKM_SUBDEV_NR; i++) {
		if (!nvkm_device_subdev(dev, order[i].engine))
			continue;

		nvkm_mask(dev, 0x20200 + order[i].offset, 0x00ff, 0x0045);
	}
}

static int
gm107_fan_pwm_ctrl(struct nvkm_therm *therm, int line, bool enable)
{
	/* nothing to do, it seems hardwired */
	return 0;
}

static int
gm107_fan_pwm_get(struct nvkm_therm *therm, int line, u32 *divs, u32 *duty)
{
	struct nvkm_device *device = therm->subdev.device;
	*divs = nvkm_rd32(device, 0x10eb20) & 0x1fff;
	*duty = nvkm_rd32(device, 0x10eb24) & 0x1fff;
	return 0;
}

static int
gm107_fan_pwm_set(struct nvkm_therm *therm, int line, u32 divs, u32 duty)
{
	struct nvkm_device *device = therm->subdev.device;
	nvkm_mask(device, 0x10eb10, 0x1fff, divs); /* keep the high bits */
	nvkm_wr32(device, 0x10eb14, duty | 0x80000000);
	return 0;
}

static int
gm107_fan_pwm_clock(struct nvkm_therm *therm, int line)
{
	return therm->subdev.device->crystal * 1000;
}

const struct gf100_idle_filter gm107_idle_filter = {
	.fecs = 0x00000000,
	.hubmmu = 0x00000000,
};

static const struct nvkm_therm_func
gm107_therm = {
	.init = gf119_therm_init,
	.fini = g84_therm_fini,
	.pwm_ctrl = gm107_fan_pwm_ctrl,
	.pwm_get = gm107_fan_pwm_get,
	.pwm_set = gm107_fan_pwm_set,
	.pwm_clock = gm107_fan_pwm_clock,
	.temp_get = g84_temp_get,
	.fan_sense = gt215_therm_fan_sense,
	.program_alarms = nvkm_therm_program_alarms_polling,
	.clkgate_init = gf100_clkgate_init,
	.clkgate_enable = gm107_clkgate_enable,
	.clkgate_fini = gk104_clkgate_fini,
};

int
gm107_therm_new(struct nvkm_device *device, int index,
		struct nvkm_therm **ptherm)
{
	return gk104_therm_new_(&gm107_therm, device, index,
				gk104_clkgate_engine_info, &gm107_idle_filter,
				ptherm);
}
