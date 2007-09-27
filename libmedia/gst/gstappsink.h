/* GStreamer
 * Copyright (C) 2007 David Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GST_APP_SINK_H_
#define _GST_APP_SINK_H_

#define UNUSEDPAR(x)  { x = x; }

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

G_BEGIN_DECLS

#define GST_TYPE_APP_SINK \
  (gst_app_sink_get_type())
#define GST_APP_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_APP_SINK,GstAppSink))
#define GST_APP_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_APP_SINK,GstAppSinkClass))
#define GST_IS_APP_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_APP_SINK))
#define GST_IS_APP_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_APP_SINK))

typedef struct _GstAppSink GstAppSink;
typedef struct _GstAppSinkClass GstAppSinkClass;

struct _GstAppSink
{
  GstBaseSink basesink;

  /*< private >*/
  GstCaps *caps;

  GCond *cond;
  GMutex *mutex;
  GQueue *queue;
  gboolean end_of_stream;
};

struct _GstAppSinkClass
{
  GstBaseSinkClass basesink_class;
};

GType gst_app_sink_get_type(void);

GST_DEBUG_CATEGORY_EXTERN (app_sink_debug);

void gst_app_sink_set_caps (GstAppSink *appsink, GstCaps *caps);
gboolean gst_app_sink_end_of_stream (GstAppSink *appsink);
GstBuffer *gst_app_sink_pull_buffer (GstAppSink *appsink);

G_END_DECLS

#endif

