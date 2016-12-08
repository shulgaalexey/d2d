#include "d2d-chat.h"

typedef enum {
	MESSAGE_BUBBLE_NONE = 0,
	MESSAGE_BUBBLE_SENT,
	MESSAGE_BUBBLE_RECEIVE,
	MESSAGE_BUBBLE_LAST
} Message_Bubble_Style;


typedef struct appdata {
	const char *name;
		Evas_Object *win;
		Evas_Object *conform;
		Evas_Object *nf;
		Evas_Object *main_box;
		Evas_Object *bubble_scroller;
		Evas_Object *bubble_box;
		Evas_Object *input_field_entry;
		int num_of_bubbles;
		int total_messages;
} appdata_s;

static const char *bubble_messages[] = {
	"Good morning, Princess?",
	"It's you again. How do you do this?",
	"This is the princess who fell from the sky into my arms.",
	"Yes. We keep meeting like this. You just suddenly show up.",
	"We could make plans to meet.",
	"No, it's nicer this way. I hope we meet again suddenly.",
	"Don't you know that it takes so little to make me happy?",
	"Good morning, Princess!  ",
	"This is incredible. You owe me an explanation.",
	"No, you're the one who owes me an explanation. You've really got a crush on me. Where shall we go, Princess?",
	NULL
};

static const char *bubble_times[] = {
	"10:00 AM", "10:01 AM", "10:02 AM", "10:03 AM", "10:04 AM", "10:05 AM",
	"3:50 PM", "3:51 PM", "3:52 PM", "3:53 PM", NULL
};

static void
win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void
bubble_button_mouse_down_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg = (Evas_Object *)data;
	int r, g, b, a;

	if (!bg)
		return;

	evas_object_color_get(bg, &r, &g, &b, &a);
	r *= 0.8;
	g *= 0.8;
	b *= 0.8;

	evas_object_color_set(bg, r, g, b, a);
}

static void
bubble_button_mouse_up_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg = (Evas_Object *)data;
	int r, g, b, a;

	if (!bg)
		return;

	evas_object_color_get(bg, &r, &g, &b, &a);
	r *= 1.25;
	g *= 1.25;
	b *= 1.25;

	evas_object_color_set(bg, r, g, b, a);
}

static Evas_Object *
create_bubble_table(Evas_Object *parent, Message_Bubble_Style style, const char *main_text, const char *sub_text)
{
	Evas_Object *bubble_table, *button, *bg, *main_label, *sub_label;
	Eina_Strbuf *strbuf = NULL;
	char *buf = NULL;

	bubble_table = elm_table_add(parent);
	evas_object_size_hint_weight_set(bubble_table, EVAS_HINT_EXPAND, 0.0);
	elm_table_padding_set(bubble_table, ELM_SCALE_SIZE(5), ELM_SCALE_SIZE(5));
	evas_object_show(bubble_table);

	button = elm_button_add(bubble_table);
	elm_object_style_set(button, "transparent");
	evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);

	bg = evas_object_rectangle_add(evas_object_evas_get(button));
	elm_object_content_set(button, bg);
	evas_object_event_callback_add(button, EVAS_CALLBACK_MOUSE_DOWN, bubble_button_mouse_down_cb, bg);
	evas_object_event_callback_add(button, EVAS_CALLBACK_MOUSE_UP, bubble_button_mouse_up_cb, bg);
	evas_object_show(button);

	/* Prepare string buffer for making mark up text */
	strbuf = eina_strbuf_new();
	/* Make a mark up text to string buffer for main label widget */
	eina_strbuf_append_printf(strbuf, BUBBLE_MAIN_TEXT_STYLE, main_text);
	/* Get the string from string buffer. String buffer will be empty. */
	buf = eina_strbuf_string_steal(strbuf);

	main_label = elm_label_add(bubble_table);
	elm_object_text_set(main_label, buf);
	elm_label_wrap_width_set(main_label, ELM_SCALE_SIZE(BUBBLE_TEXT_WIDTH));
	elm_label_line_wrap_set(main_label, ELM_WRAP_MIXED);
	evas_object_size_hint_weight_set(main_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(main_label, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_repeat_events_set(main_label, EINA_TRUE);
	evas_object_show(main_label);

	/* Make a mark up text to string buffer for sub label widget */
	eina_strbuf_append_printf(strbuf, BUBBLE_SUB_TEXT_STYLE, sub_text);
	/* Get the string from string buffer. String buffer will be empty. */
	buf = eina_strbuf_string_steal(strbuf);
	/* Free string buffer */
	eina_strbuf_free(strbuf);

	sub_label = elm_label_add(bubble_table);
	elm_object_text_set(sub_label, buf);
	evas_object_size_hint_weight_set(sub_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_repeat_events_set(sub_label, EINA_TRUE);
	evas_object_show(sub_label);

	switch (style) {
		case MESSAGE_BUBBLE_SENT:
			evas_object_size_hint_align_set(bubble_table, 1.0, 0.0);
			evas_object_size_hint_align_set(sub_label, 1.0, EVAS_HINT_FILL);
			evas_object_color_set(bg, 200, 170, 100, 255);
			elm_table_pack(bubble_table, button, 0, 0, 1, 2);
			elm_table_pack(bubble_table, main_label, 0, 0, 1, 1);
			elm_table_pack(bubble_table, sub_label, 0, 1, 1, 1);
			break;
		case MESSAGE_BUBBLE_RECEIVE:
			evas_object_size_hint_align_set(bubble_table, 0.0, 0.0);
			evas_object_size_hint_align_set(sub_label, 0.0, EVAS_HINT_FILL);
			evas_object_color_set(bg, 100, 170, 200, 255);
			elm_table_pack(bubble_table, button, 0, 0, 1, 2);
			elm_table_pack(bubble_table, main_label, 0, 0, 1, 1);
			elm_table_pack(bubble_table, sub_label, 0, 1, 1, 1);
			break;
		case MESSAGE_BUBBLE_NONE:
		case MESSAGE_BUBBLE_LAST:
		default:
			break;
	}

	return bubble_table;
}

static void
load_messages(appdata_s *ad)
{
	Evas_Object *bubble_table;

	ad->total_messages = NUM_OF_SAMPLE_MESSAGES;
	ad->num_of_bubbles = 0;
	int count = ad->total_messages - 1;

	while (count >= 0) {
		bubble_table = create_bubble_table(ad->bubble_box, count % 2 + 1,
				bubble_messages[count],
				bubble_times[count]);
		evas_object_show(bubble_table);
		elm_box_pack_start(ad->bubble_box, bubble_table);
		ad->num_of_bubbles++;
		count--;
	}
}

static char *
get_current_time()
{
	Eina_Strbuf *strbuf = eina_strbuf_new();
	time_t local_time = time(NULL);
	char buf[200] = {0};
	char *ret = NULL;
	struct tm *time_info = localtime(&local_time);

	strftime(buf, 200, "%l:%M", time_info);
	eina_strbuf_append_printf(strbuf, "%s %s", buf, (time_info->tm_hour >= 12) ? "PM":"AM");
	ret = eina_strbuf_string_steal(strbuf);
	eina_strbuf_free(strbuf);

	return ret;
}

static void
bubble_box_resize_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Coord w, h;
	appdata_s *ad = data;

	elm_scroller_child_size_get(ad->bubble_scroller, &w, &h);
	elm_scroller_region_show(ad->bubble_scroller, 0, h, 0, 0);
	evas_object_event_callback_del(ad->bubble_box, EVAS_CALLBACK_RESIZE, bubble_box_resize_cb);
}

static void
send_message(appdata_s *ad)
{
	Evas_Object *bubble_table;
	const char *main_text = NULL;

	if (!ad->input_field_entry)
		return;
	main_text = elm_entry_entry_get(ad->input_field_entry);
	if (!main_text || (strlen(main_text) == 0))
		return;

	bubble_table = create_bubble_table(ad->bubble_box, MESSAGE_BUBBLE_SENT,
			elm_entry_entry_get(ad->input_field_entry),
			get_current_time());
	evas_object_show(bubble_table);
	elm_box_pack_end(ad->bubble_box, bubble_table);
	ad->num_of_bubbles++;
	ad->total_messages++;
	elm_entry_entry_set(ad->input_field_entry, "");

	evas_object_event_callback_add(ad->bubble_box, EVAS_CALLBACK_RESIZE, bubble_box_resize_cb, ad);
}

static void
send_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	send_message(data);
}

static void
entry_focused_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg = (Evas_Object *)data;
	evas_object_color_set(bg, 110, 210, 210, 255);
}

static void
entry_unfocused_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *bg = (Evas_Object *)data;
	evas_object_color_set(bg, 0, 0, 0, 0);
}

static Evas_Object *
create_input_field_table(appdata_s *ad)
{
	Evas_Object *table, *button, *bg;

	table = elm_table_add(ad->main_box);
	elm_table_homogeneous_set(table, EINA_TRUE);
	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(table, EVAS_HINT_FILL, 1.0);
	evas_object_show(table);

	button = elm_button_add(table);
	elm_object_style_set(button, "transparent");
	evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);

	bg = evas_object_rectangle_add(evas_object_evas_get(button));
	elm_object_content_set(button, bg);
	evas_object_color_set(bg, 120, 220, 220, 255);
	evas_object_show(button);
	elm_table_pack(table, button, 0, 0, 3, 2);

	button = elm_button_add(table);
	elm_object_style_set(button, "transparent");
	evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);

	bg = evas_object_rectangle_add(evas_object_evas_get(button));
	elm_object_content_set(button, bg);
	evas_object_color_set(bg, 0, 0, 0, 0);
	evas_object_show(button);
	elm_table_pack(table, button, 0, 0, 2, 2);

	ad->input_field_entry = elm_entry_add(table);
	elm_object_part_text_set(ad->input_field_entry, "elm.guide", "Enter Message");
	evas_object_size_hint_weight_set(ad->input_field_entry, EVAS_HINT_EXPAND, 0.0);
	evas_object_size_hint_align_set(ad->input_field_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_smart_callback_add(ad->input_field_entry, "focused", entry_focused_cb, bg);
	evas_object_smart_callback_add(ad->input_field_entry, "unfocused", entry_unfocused_cb, bg);
	evas_object_show(ad->input_field_entry);
	elm_table_pack(table, ad->input_field_entry, 0, 0, 2, 2);

	button = elm_button_add(table);
	evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_text_set(button, "SEND");
	evas_object_smart_callback_add(button, "clicked", send_button_clicked_cb, ad);
	evas_object_show(button);
	elm_table_pack(table, button, 2, 1, 1, 1);

	return table;
}

static Evas_Object *
create_main_view(appdata_s *ad)
{
	Evas_Object *main_scroller, *input_field_table;

	main_scroller = elm_scroller_add(ad->nf);
	elm_scroller_bounce_set(main_scroller, EINA_FALSE, EINA_TRUE);
	evas_object_size_hint_weight_set(main_scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(main_scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(main_scroller);

	ad->main_box = elm_box_add(main_scroller);
	elm_box_align_set(ad->main_box, 0, 0);
	evas_object_size_hint_weight_set(ad->main_box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(ad->main_box);

	ad->bubble_scroller = elm_scroller_add(ad->main_box);
	elm_scroller_bounce_set(ad->bubble_scroller, EINA_FALSE, EINA_TRUE);
	evas_object_size_hint_weight_set(ad->bubble_scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->bubble_scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);

	ad->bubble_box = elm_box_add(ad->bubble_scroller);
	elm_box_align_set(ad->bubble_box, 0, 0);
	evas_object_size_hint_weight_set(ad->bubble_box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(ad->bubble_box);
	elm_box_padding_set(ad->bubble_box, ELM_SCALE_SIZE(10), ELM_SCALE_SIZE(15));

	elm_object_content_set(ad->bubble_scroller, ad->bubble_box);
	evas_object_show(ad->bubble_scroller);
	elm_box_pack_end(ad->main_box, ad->bubble_scroller);

	input_field_table = create_input_field_table(ad);
	evas_object_show(input_field_table);
	elm_box_pack_end(ad->main_box, input_field_table);
	elm_object_content_set(main_scroller, ad->main_box);

	load_messages(ad);

	return main_scroller;
}

static void
create_base_gui(appdata_s *ad)
{
	Evas_Object *main_scroller, *bg;

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_app_base_scale_set(1.8);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	/* Indicator BG */
	bg = elm_bg_add(ad->conform);
	elm_object_style_set(bg, "indicator/headerbg");
	elm_object_part_content_set(ad->conform, "elm.swallow.indicator_bg", bg);
	evas_object_show(bg);

	/* Naviframe */
	ad->nf = elm_naviframe_add(ad->conform);
	evas_object_size_hint_weight_set(ad->nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->conform, ad->nf);
	evas_object_show(ad->nf);

	/* Main view */
	main_scroller = create_main_view(ad);
	elm_naviframe_item_push(ad->nf, "Message Bubble", NULL, NULL, main_scroller, NULL);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
	   Initialize UI resources and application's data
	   If this function returns true, the main loop of application starts
	   If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control , void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	}

	return ret;
}
