#ifndef _LINUX_PSMOUSE_H
#define _LINUX_PSMOUSE_H

#include <linux/types.h>

struct psmouse;

enum psmouse_type {
	PSMOUSE_NONE = 0,
	PSMOUSE_PS2,
	PSMOUSE_PS2PP,
	PSMOUSE_THINKPS,
	PSMOUSE_GENPS,
	PSMOUSE_IMPS,
	PSMOUSE_IMEX,
	PSMOUSE_SYNAPTICS,
	PSMOUSE_ALPS,
	PSMOUSE_LIFEBOOK,
	PSMOUSE_TRACKPOINT,
	PSMOUSE_TOUCHKIT_PS2,
	PSMOUSE_CORTRON,
	PSMOUSE_HGPK,
	PSMOUSE_ELANTECH,
	PSMOUSE_FSP,
	PSMOUSE_SYNAPTICS_RELATIVE,
	PSMOUSE_CYPRESS,
	PSMOUSE_FOCALTECH,
	PSMOUSE_VMMOUSE,
	PSMOUSE_AUTO		/* This one should always be last */
};

enum psmouse_type psmouse_get_type(struct psmouse *psmouse);
void psmouse_pt_btns_enable(struct psmouse *psmouse);
void psmouse_pt_btns_report(struct psmouse *psmouse, int code, bool value);
void psmouse_pt_btns_sync(struct psmouse *psmouse);
void psmouse_pt_btns_clear(struct psmouse *psmouse);

#endif /* _LINUX_PSMOUSE_H */
