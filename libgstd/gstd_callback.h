/*
 * This file is part of GStreamer Daemon
 * Copyright 2015-2022 Ridgerun, LLC (http://www.ridgerun.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GSTD_CALLBACK_H__
#define __GSTD_CALLBACK_H__

#include <glib-object.h>

#include "gstd_object.h"

G_BEGIN_DECLS
/*
 * Type declaration.
 */
#define GSTD_TYPE_CALLBACK \
  (gstd_callback_get_type())
#define GSTD_CALLBACK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GSTD_TYPE_CALLBACK,GstdCallback))
#define GSTD_CALLBACK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GSTD_TYPE_CALLBACK,GstdCallbackClass))
#define GSTD_IS_CALLBACK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GSTD_TYPE_CALLBACK))
#define GSTD_IS_CALLBACK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GSTD_TYPE_CALLBACK))
#define GSTD_CALLBACK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), GSTD_TYPE_CALLBACK, GstdCallbackClass))
typedef struct _GstdCallback GstdCallback;
typedef struct _GstdCallbackClass GstdCallbackClass;
GType gstd_callback_get_type (void);

struct _GstdCallback
{
  GstdObject parent;

  gchar *signal_name;
  GValue *param_values;
  guint n_params;
};

struct _GstdCallbackClass
{
  GstdObjectClass parent_class;
};


GstdCallback *gstd_callback_new (const gchar * signal_name,
    GValue * return_value, guint n_param_values, const GValue * param_values);

G_END_DECLS
#endif // __GSTD_CALLBACK_H__
