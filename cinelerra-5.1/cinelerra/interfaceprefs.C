
/*
 * CINELERRA
 * Copyright (C) 2008 Adam Williams <broadcast at earthling dot net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "deleteallindexes.h"
#include "edl.h"
#include "edlsession.h"
#include "file.h"
#include "filesystem.h"
#include "language.h"
#include "mwindow.h"
#include "preferences.h"
#include "preferencesthread.h"
#include "probeprefs.h"
#include "interfaceprefs.h"
#include "shbtnprefs.h"
#include "theme.h"

#if 0
N_("Drag all following edits")
N_("Drag only one edit")
N_("Drag source only")
N_("No effect")
#endif

#define MOVE_ALL_EDITS_TITLE N_("Drag all following edits")
#define MOVE_ONE_EDIT_TITLE N_("Drag only one edit")
#define MOVE_NO_EDITS_TITLE N_("Drag source only")
#define MOVE_EDITS_DISABLED_TITLE N_("No effect")

InterfacePrefs::InterfacePrefs(MWindow *mwindow, PreferencesWindow *pwindow)
 : PreferencesDialog(mwindow, pwindow)
{
	min_db = 0;
	max_db = 0;
	shbtn_dialog = 0;
	file_probe_dialog = 0;
}

InterfacePrefs::~InterfacePrefs()
{
	delete min_db;
	delete max_db;
	delete shbtn_dialog;
	delete file_probe_dialog;
}


void InterfacePrefs::create_objects()
{
	BC_Resources *resources = BC_WindowBase::get_resources();
	int margin = mwindow->theme->widget_border;
	char string[BCTEXTLEN];
	int x0 = mwindow->theme->preferencesoptions_x;
	int y0 = mwindow->theme->preferencesoptions_y;
	int x = x0, y = y0;

	add_subwindow(new BC_Title(x, y, _("Editing:"), LARGEFONT,
		resources->text_default));
	y += 35;

	int x2 = get_w()/2, y2 = y;
	x = x2;
	BC_Title *title;
	add_subwindow(title = new BC_Title(x, y, _("Keyframe reticle:")));
	y += title->get_h() + 5;
	keyframe_reticle = new KeyframeReticle(pwindow, this, x, y,
		&pwindow->thread->preferences->keyframe_reticle);
	add_subwindow(keyframe_reticle);
	keyframe_reticle->create_objects();

	y += 30;
	add_subwindow(title = new BC_Title(x, y, _("Snapshot path:")));
	y += title->get_h() + 5;
	add_subwindow(snapshot_path = new SnapshotPathText(pwindow, this, x, y, get_w()-x-30));

	x = x0;  y = y2;
	add_subwindow(new BC_Title(x, y, _("Clicking on edit boundaries does what:")));
	y += 25;
	add_subwindow(new BC_Title(x, y, _("Button 1:")));

	int x1 = x + 100;
	ViewBehaviourText *text;
	add_subwindow(text = new ViewBehaviourText(x1, y - 5,
		behavior_to_text(pwindow->thread->edl->session->edit_handle_mode[0]),
			pwindow,
			&(pwindow->thread->edl->session->edit_handle_mode[0])));
	text->create_objects();
	y += 30;
	add_subwindow(new BC_Title(x, y, _("Button 2:")));
	add_subwindow(text = new ViewBehaviourText(x1,
		y - 5,
		behavior_to_text(pwindow->thread->edl->session->edit_handle_mode[1]),
			pwindow,
			&(pwindow->thread->edl->session->edit_handle_mode[1])));
	text->create_objects();
	y += 30;
	add_subwindow(new BC_Title(x, y, _("Button 3:")));
	add_subwindow(text = new ViewBehaviourText(x1, y - 5,
		behavior_to_text(pwindow->thread->edl->session->edit_handle_mode[2]),
			pwindow,
			&(pwindow->thread->edl->session->edit_handle_mode[2])));
	text->create_objects();
	y += text->get_h() + 30;

	x = x0;
	add_subwindow(new BC_Bar(5, y, 	get_w() - 10));
	y += 5;
	add_subwindow(new BC_Title(x, y, _("Operation:"), LARGEFONT,
		resources->text_default));
	y += 35;

	int y1 = y;
	AndroidRemote *android_remote = new AndroidRemote(pwindow, x2, y);
	add_subwindow(android_remote);
	y += android_remote->get_h() + 10;
	add_subwindow(title = new BC_Title(x2, y, _("Port:")));
	int x3 = x2 + title->get_w() + margin;
	AndroidPort *android_port = new AndroidPort(pwindow, x3, y);
	add_subwindow(android_port);
	y += title->get_h() + 10;
	add_subwindow(title = new BC_Title(x2, y, _("PIN:")));
	AndroidPIN *android_pin = new AndroidPIN(pwindow, x3, y);
	add_subwindow(android_pin);
	y += title->get_h() + 30;

	ShBtnPrefs *shbtn_prefs = new ShBtnPrefs(pwindow, this, x2, y);
	add_subwindow(shbtn_prefs);
	y += shbtn_prefs->get_h() + 30;

	y2 = y;
	x = x0;  y = y1;
	add_subwindow(file_probes = new PrefsFileProbes(pwindow, this, x, y));
	y += 30;

	PrefsTrapSigSEGV *trap_segv = new PrefsTrapSigSEGV(this, x, y);
	add_subwindow(trap_segv);
	x1 = x + trap_segv->get_w() + 10;
	add_subwindow(new BC_Title(x1, y, _("(must be root)"), MEDIUMFONT, RED));
	y += 30;

	PrefsTrapSigINTR *trap_intr = new PrefsTrapSigINTR(this, x, y);
	add_subwindow(trap_intr);
	add_subwindow(new BC_Title(x1, y, _("(must be root)"), MEDIUMFONT, RED));
	y += 30;

	yuv420p_dvdlace = new PrefsYUV420P_DVDlace(pwindow, this, x, y);
	add_subwindow(yuv420p_dvdlace);
	y += 30;

	if( y2 > y ) y = y2;
	add_subwindow(title = new BC_Title(x, y + 5, _("Min DB for meter:")));
	x += title->get_w() + 10;
	sprintf(string, "%d", pwindow->thread->edl->session->min_meter_db);
	add_subwindow(min_db = new MeterMinDB(pwindow, string, x, y));
	x += min_db->get_w() + 10;
	add_subwindow(title = new BC_Title(x, y + 5, _("Max DB:")));
	x += title->get_w() + 10;
	sprintf(string, "%d", pwindow->thread->edl->session->max_meter_db);
	add_subwindow(max_db = new MeterMaxDB(pwindow, string, x, y));
	y += 30;

	StillImageUseDuration *use_stduration = new StillImageUseDuration(pwindow,
		pwindow->thread->edl->session->si_useduration, x2, y2);
	add_subwindow(use_stduration);
	int tw = 0, th = 0;
	BC_CheckBox::calculate_extents(this, &tw, &th, 0, 0);
	x2 += tw + 3;
	y2 += use_stduration->get_h() + 3;
	StillImageDuration *stduration = new StillImageDuration(pwindow, x2, y2);
	add_subwindow(stduration);
	x2 += stduration->get_w() + 10;
	y2 += 3;
	add_subwindow(new BC_Title(x2, y2, _("Seconds")));
	y2 += 30;

	x = x0;  y = y2;
	add_subwindow(new BC_Bar(5, y, 	get_w() - 10));
	y += 5;


	add_subwindow(new BC_Title(x, y, _("Index files:"), LARGEFONT, resources->text_default));
	y += 30;

	add_subwindow(new BC_Title(x, y + 5,
		_("Index files go here:"), MEDIUMFONT, resources->text_default));
	x1 = x + 230;
	add_subwindow(ipathtext = new IndexPathText(x1, y, pwindow,
		pwindow->thread->preferences->index_directory));
	x1 +=  ipathtext->get_w();
	add_subwindow(ipath = new BrowseButton(mwindow->theme, this, ipathtext, x1, y,
		pwindow->thread->preferences->index_directory,
		_("Index Path"), _("Select the directory for index files"), 1));

	y += 30;
	add_subwindow(new BC_Title(x, y + 5, _("Size of index file:"),
		MEDIUMFONT, resources->text_default));
	sprintf(string, "%jd", pwindow->thread->preferences->index_size);
	add_subwindow(isize = new IndexSize(x + 230, y, pwindow, string));
	add_subwindow(new ScanCommercials(pwindow, 400,y));

	y += 30;
	add_subwindow(new BC_Title(x, y + 5, _("Number of index files to keep:"),
		MEDIUMFONT, resources->text_default));
	sprintf(string, "%ld", (long)pwindow->thread->preferences->index_count);
	add_subwindow(icount = new IndexCount(x + 230, y, pwindow, string));
	add_subwindow(deleteall = new DeleteAllIndexes(mwindow, pwindow, 400, y));
	y += 30;
	add_subwindow(ffmpeg_marker_files = new IndexFFMPEGMarkerFiles(this, x, y));
	y += 35;
}

const char* InterfacePrefs::behavior_to_text(int mode)
{
	switch(mode) {
		case MOVE_ALL_EDITS: return _(MOVE_ALL_EDITS_TITLE);
		case MOVE_ONE_EDIT:  return _(MOVE_ONE_EDIT_TITLE);
		case MOVE_NO_EDITS:  return _(MOVE_NO_EDITS_TITLE);
		case MOVE_EDITS_DISABLED: return _(MOVE_EDITS_DISABLED_TITLE);
		default: return "";
	}
}

IndexPathText::IndexPathText(int x, int y, PreferencesWindow *pwindow, char *text)
 : BC_TextBox(x, y, 240, 1, text)
{
	this->pwindow = pwindow;
}

IndexPathText::~IndexPathText() {}

int IndexPathText::handle_event()
{
	strcpy(pwindow->thread->preferences->index_directory, get_text());
	return 1;
}




IndexSize::IndexSize(int x, int y, PreferencesWindow *pwindow, char *text)
 : BC_TextBox(x, y, 100, 1, text)
{
	this->pwindow = pwindow;
}

int IndexSize::handle_event()
{
	long result;

	result = atol(get_text());
	if(result < 64000) result = 64000;
	//if(result < 500000) result = 500000;
	pwindow->thread->preferences->index_size = result;
	return 0;
}



IndexCount::IndexCount(int x, int y, PreferencesWindow *pwindow, char *text)
 : BC_TextBox(x, y, 100, 1, text)
{
	this->pwindow = pwindow;
}

int IndexCount::handle_event()
{
	long result;

	result = atol(get_text());
	if(result < 1) result = 1;
	pwindow->thread->preferences->index_count = result;
	return 0;
}



IndexFFMPEGMarkerFiles::IndexFFMPEGMarkerFiles(InterfacePrefs *iface_prefs, int x, int y)
 : BC_CheckBox(x, y,
	iface_prefs->pwindow->thread->preferences->ffmpeg_marker_indexes,
	_("build ffmpeg marker indexes"))
{
	this->iface_prefs = iface_prefs;
}
IndexFFMPEGMarkerFiles::~IndexFFMPEGMarkerFiles()
{
}

int IndexFFMPEGMarkerFiles::handle_event()
{
	iface_prefs->pwindow->thread->preferences->ffmpeg_marker_indexes = get_value();
	return 1;
}


ViewBehaviourText::ViewBehaviourText(int x, int y, const char *text, PreferencesWindow *pwindow,
	int *output)
 : BC_PopupMenu(x, y, 200, text)
{
	this->output = output;
}

ViewBehaviourText::~ViewBehaviourText()
{
}

int ViewBehaviourText::handle_event()
{
	return 0;
}

void ViewBehaviourText::create_objects()
{
	add_item(new ViewBehaviourItem(this, _(MOVE_ALL_EDITS_TITLE), MOVE_ALL_EDITS));
	add_item(new ViewBehaviourItem(this, _(MOVE_ONE_EDIT_TITLE), MOVE_ONE_EDIT));
	add_item(new ViewBehaviourItem(this, _(MOVE_NO_EDITS_TITLE), MOVE_NO_EDITS));
	add_item(new ViewBehaviourItem(this, _(MOVE_EDITS_DISABLED_TITLE), MOVE_EDITS_DISABLED));
}


ViewBehaviourItem::ViewBehaviourItem(ViewBehaviourText *popup, char *text, int behaviour)
 : BC_MenuItem(text)
{
	this->popup = popup;
	this->behaviour = behaviour;
}

ViewBehaviourItem::~ViewBehaviourItem()
{
}

int ViewBehaviourItem::handle_event()
{
	popup->set_text(get_text());
	*(popup->output) = behaviour;
	return 1;
}


MeterMinDB::MeterMinDB(PreferencesWindow *pwindow, char *text, int x, int y)
 : BC_TextBox(x, y, 50, 1, text)
{
	this->pwindow = pwindow;
}

int MeterMinDB::handle_event()
{
	pwindow->thread->redraw_meters = 1;
	pwindow->thread->edl->session->min_meter_db = atol(get_text());
	return 0;
}


MeterMaxDB::MeterMaxDB(PreferencesWindow *pwindow, char *text, int x, int y)
 : BC_TextBox(x, y, 50, 1, text)
{
	this->pwindow = pwindow;
}

int MeterMaxDB::handle_event()
{
	pwindow->thread->redraw_meters = 1;
	pwindow->thread->edl->session->max_meter_db = atol(get_text());
	return 0;
}


ScanCommercials::ScanCommercials(PreferencesWindow *pwindow, int x, int y)
 : BC_CheckBox(x,
 	y,
	pwindow->thread->preferences->scan_commercials,
	_("Scan for commercials during toc build"))
{
	this->pwindow = pwindow;
}
int ScanCommercials::handle_event()
{
	pwindow->thread->preferences->scan_commercials = get_value();
	return 1;
}


AndroidRemote::AndroidRemote(PreferencesWindow *pwindow, int x, int y)
 : BC_CheckBox(x, y,
	pwindow->thread->preferences->android_remote,
	_("Android Remote Control"))
{
	this->pwindow = pwindow;
}
int AndroidRemote::handle_event()
{
	pwindow->thread->preferences->android_remote = get_value();
	return 1;
}

AndroidPIN::AndroidPIN(PreferencesWindow *pwindow, int x, int y)
 : BC_TextBox(x, y, 240, 1, pwindow->thread->preferences->android_pin)
{
	this->pwindow = pwindow;
}

int AndroidPIN::handle_event()
{
	char *txt = pwindow->thread->preferences->android_pin;
	int len = sizeof(pwindow->thread->preferences->android_pin);
	strncpy(txt, get_text(), len);
	return 1;
}


AndroidPort::AndroidPort(PreferencesWindow *pwindow, int x, int y)
 : BC_TextBox(x, y, 72, 1, pwindow->thread->preferences->android_port)
{
	this->pwindow = pwindow;
}

int AndroidPort::handle_event()
{
	unsigned short port = atoi(get_text());
	if( port < 1024 ) port = 1024;
	pwindow->thread->preferences->android_port = port;
	char str[BCSTRLEN];
	sprintf(str,"%u",port);
	update(str);
	return 1;
}

int InterfacePrefs::start_shbtn_dialog()
{
	if( !shbtn_dialog )
		shbtn_dialog = new ShBtnEditDialog(pwindow);
	shbtn_dialog->start();
	return 1;
}

ShBtnPrefs::ShBtnPrefs(PreferencesWindow *pwindow, InterfacePrefs *iface_prefs, int x, int y)
 : BC_GenericButton(x, y, _("Shell Commands"))
{
	this->pwindow = pwindow;
	this->iface_prefs = iface_prefs;
	set_tooltip(_("Main Menu Shell Commands"));
}

int ShBtnPrefs::handle_event()
{
	return iface_prefs->start_shbtn_dialog();
}


StillImageUseDuration::StillImageUseDuration(PreferencesWindow *pwindow, int value, int x, int y)
 : BC_CheckBox(x, y, value, _("Import images with a duration of"))
{
	this->pwindow = pwindow;
}

int StillImageUseDuration::handle_event()
{
	pwindow->thread->edl->session->si_useduration = get_value();
	return 1;
}

StillImageDuration::StillImageDuration(PreferencesWindow *pwindow, int x, int y)
 : BC_TextBox(x, y, 70, 1, pwindow->thread->edl->session->si_duration)
{
	this->pwindow = pwindow;
}
int StillImageDuration::handle_event()
{
	pwindow->thread->edl->session->si_duration = atof(get_text());
	return 1;
}


HairlineItem::HairlineItem(KeyframeReticle *popup, int hairline)
 : BC_MenuItem(popup->hairline_to_string(hairline))
{
	this->popup = popup;
	this->hairline = hairline;
}

HairlineItem::~HairlineItem()
{
}

int HairlineItem::handle_event()
{
	popup->pwindow->thread->redraw_overlays = 1;
	popup->set_text(get_text());
	*(popup->output) = hairline;
	return 1;
}


KeyframeReticle::KeyframeReticle(PreferencesWindow *pwindow,
	InterfacePrefs *iface_prefs, int x, int y, int *output)
 : BC_PopupMenu(x, y, 220, hairline_to_string(*output))
{
	this->pwindow = pwindow;
	this->iface_prefs = iface_prefs;
	this->output = output;
}

KeyframeReticle::~KeyframeReticle()
{
}

const char *KeyframeReticle::hairline_to_string(int type)
{
	switch( type ) {
	case HAIRLINE_NEVER:    return _("Never");
	case HAIRLINE_DRAGGING:	return _("Dragging");
	case HAIRLINE_ALWAYS:   return _("Always");
	}
	return _("Unknown");
}

void KeyframeReticle::create_objects()
{
	add_item(new HairlineItem(this, HAIRLINE_NEVER));
	add_item(new HairlineItem(this, HAIRLINE_DRAGGING));
	add_item(new HairlineItem(this, HAIRLINE_ALWAYS));
}

PrefsTrapSigSEGV::PrefsTrapSigSEGV(InterfacePrefs *subwindow, int x, int y)
 : BC_CheckBox(x, y,
	subwindow->pwindow->thread->preferences->trap_sigsegv,
	_("trap sigSEGV"))
{
	this->subwindow = subwindow;
}
PrefsTrapSigSEGV::~PrefsTrapSigSEGV()
{
}
int PrefsTrapSigSEGV::handle_event()
{
	subwindow->pwindow->thread->preferences->trap_sigsegv = get_value();
	return 1;
}

PrefsTrapSigINTR::PrefsTrapSigINTR(InterfacePrefs *subwindow, int x, int y)
 : BC_CheckBox(x, y,
	subwindow->pwindow->thread->preferences->trap_sigintr,
	_("trap sigINT"))
{
	this->subwindow = subwindow;
}
PrefsTrapSigINTR::~PrefsTrapSigINTR()
{
}
int PrefsTrapSigINTR::handle_event()
{
	subwindow->pwindow->thread->preferences->trap_sigintr = get_value();
	return 1;
}


void InterfacePrefs::start_probe_dialog()
{
	if( !file_probe_dialog )
		file_probe_dialog = new FileProbeDialog(pwindow);
	file_probe_dialog->start();
}

PrefsFileProbes::PrefsFileProbes(PreferencesWindow *pwindow,
		InterfacePrefs *subwindow, int x, int y)
 : BC_GenericButton(x, y, _("Probe Order"))
{
	this->pwindow = pwindow;
	this->subwindow = subwindow;
	set_tooltip(_("File Open Probe Ordering"));
}

int PrefsFileProbes::handle_event()
{
	subwindow->start_probe_dialog();
	return 1;
}


PrefsYUV420P_DVDlace::PrefsYUV420P_DVDlace(PreferencesWindow *pwindow,
	InterfacePrefs *subwindow, int x, int y)
 : BC_CheckBox(x, y, pwindow->thread->preferences->dvd_yuv420p_interlace,
	_("Use yuv420p dvd interlace format"))
{
	this->pwindow = pwindow;
	this->subwindow = subwindow;
}

int PrefsYUV420P_DVDlace::handle_event()
{
	pwindow->thread->preferences->dvd_yuv420p_interlace = get_value();
	return 1;
}


SnapshotPathText::SnapshotPathText(PreferencesWindow *pwindow,
	InterfacePrefs *subwindow, int x, int y, int w)
 : BC_TextBox(x, y, w, 1, pwindow->thread->preferences->snapshot_path)
{
	this->pwindow = pwindow;
	this->subwindow = subwindow;
}

SnapshotPathText::~SnapshotPathText()
{
}

int SnapshotPathText::handle_event()
{
	strcpy(pwindow->thread->preferences->snapshot_path, get_text());
	return 1;
}

