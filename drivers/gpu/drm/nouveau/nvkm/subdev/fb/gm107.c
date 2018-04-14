/*
 * Copyright 2012-2018 Red Hat Inc.
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
 * Authors: Ben Skeggs
 *          Lyude Paul
 */
#include "gf100.h"
#include "gk104.h"
#include "gk110.h"
#include "ram.h"

/*
 *******************************************************************************
 * PGRAPH registers for clockgating
 *******************************************************************************
 */
static const struct nvkm_therm_clkgate_init
gm107_fb_clkgate_slcg_init_unk_0[] = {
	{ 0x100d14, 1, 0x00000000 },
	{}
};

static const struct nvkm_therm_clkgate_init
gm107_fb_clkgate_slcg_init_vm_0[] = {
	{ 0x100c9c, 1, 0x00000000 },
	{}
};

static const struct nvkm_therm_clkgate_init
gm107_fb_clkgate_slcg_init_bcast_0[] = {
	{ 0x17e050, 1, 0x00000000 },
	{ 0x17e35c, 1, 0x00000000 },
	{ 0x10f280, 1, 0x00000000 },
	{}
};

static const struct nvkm_therm_clkgate_init
gm107_fb_clkgate_slcg_init_pxbar_0[] = {
	{ 0x13c824, 1, 0x00000000 },
	{ 0x13cbe4, 1, 0x00000000 },
	{}
};

static const struct nvkm_therm_clkgate_init
gm107_fb_clkgate_blcg_init_main_0[] = {
	{ 0x10f000, 1, 0x00000043 },
	{ 0x17e030, 1, 0x00000044 },
	{ 0x17e040, 1, 0x00000044 },
	{}
};

static const struct nvkm_therm_clkgate_init
gm107_fb_clkgate_blcg_init_unk_1[] = {
	{ 0x17e3e0, 1, 0x00000044 },
	{ 0x17e3c8, 1, 0x00000044 },
	/* XXX extremely unlikely to be a BLCG/SLCG register */
	/*{ 0x17e278, 1, 0x0007fd60 },*/
	{}
};

static const struct nvkm_therm_clkgate_pack
gm107_fb_clkgate_pack[] = {
	{ gm107_fb_clkgate_slcg_init_unk_0 },
	{ gm107_fb_clkgate_slcg_init_vm_0 },
	{ gm107_fb_clkgate_slcg_init_bcast_0 },
	{ gm107_fb_clkgate_slcg_init_pxbar_0 },
	{ gk110_fb_clkgate_blcg_init_unk_0 },
	{ gk104_fb_clkgate_blcg_init_vm_0 },
	{ gm107_fb_clkgate_blcg_init_main_0 },
	{ gm107_fb_clkgate_blcg_init_unk_1 },
	{}
};

static const struct nvkm_fb_func
gm107_fb = {
	.dtor = gf100_fb_dtor,
	.oneinit = gf100_fb_oneinit,
	.init = gf100_fb_init,
	.init_page = gf100_fb_init_page,
	.intr = gf100_fb_intr,
	.ram_new = gm107_ram_new,
	.default_bigpage = 17,
	.clkgate_pack = gm107_fb_clkgate_pack,
};

int
gm107_fb_new(struct nvkm_device *device, int index, struct nvkm_fb **pfb)
{
	return gf100_fb_new_(&gm107_fb, device, index, pfb);
}
