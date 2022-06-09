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
#include <gst/check/gstcheck.h>
#include <string.h>

#include "libgstc.h"
#include "libgstc_assert.h"
#include "libgstc_json.h"
#include "libgstc_socket.h"

/* Test Fixture */
static gchar _request[3][512];
static GstClient *_client;

static void
setup (void)
{
  const gchar *address = "";
  unsigned int port = 0;
  unsigned long wait_time = 5;
  int keep_connection_open = 0;

  gstc_client_new (address, port, wait_time, keep_connection_open, &_client);
}

static void
teardown (void)
{
  gstc_client_free (_client);
}

/* Mock implementation of a socket */
typedef struct _GstcSocket
{
} GstcSocket;

GstcSocket _socket;

GstcStatus
gstc_socket_new (const char *address, const unsigned int port,
    const int keep_connection_open, GstcSocket ** out)
{
  *out = &_socket;

  return GSTC_OK;
}

void
gstc_socket_free (GstcSocket * socket)
{
}

GstcStatus
gstc_socket_send (GstcSocket * socket, const gchar * request,
    gchar ** response, const int timeout)
{
  static int reqnum = 0;

  *response = malloc (1);

  memcpy (_request[reqnum], request, strlen (request));

  reqnum++;

  return GSTC_OK;
}

GstcStatus
gstc_json_get_int (const gchar * json, const gchar * name, gint * out)
{
  return *out = GSTC_OK;
}

GstcStatus
gstc_json_is_null (const gchar * json, const gchar * name, gint * out)
{
  *out = 0;
  return GSTC_OK;
}

GstcStatus
gstc_json_get_child_char_array (const char *json,
    const char *parent_name,
    const char *array_name,
    const char *element_name, char **out[], int *array_lenght)
{
  gstc_assert_and_ret_val (NULL != json, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != parent_name, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != array_name, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != element_name, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != out, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != array_lenght, GSTC_NULL_ARGUMENT);
  return GSTC_OK;
}

GstcStatus
gstc_json_child_string (const char *json, const char *parent_name,
    const char *data_name, char **out)
{
  gstc_assert_and_ret_val (NULL != json, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != parent_name, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != data_name, GSTC_NULL_ARGUMENT);
  gstc_assert_and_ret_val (NULL != out, GSTC_NULL_ARGUMENT);

  return GSTC_OK;
}

GST_START_TEST (test_pipeline_signal_connect_success)
{
  GstcStatus ret;
  const gchar *pipeline_name = "pipe";
  const gchar *element_name = "element_name";
  const gchar *signal_name = "signal_name";
  const gchar *expected[] = {
    "update /pipelines/pipe/elements/element_name/signals/signal_name/timeout 100",
    "read /pipelines/pipe/elements/element_name/signals/signal_name/callback"
  };
  gint signal_timeout = 100;
  gchar *response = NULL;

  ret = gstc_pipeline_signal_connect (_client, pipeline_name, element_name,
      signal_name, signal_timeout, &response);
  assert_equals_int (GSTC_OK, ret);

  assert_equals_string (expected[0], _request[0]);
  assert_equals_string (expected[1], _request[1]);

  free (response);
}

GST_END_TEST;

GST_START_TEST (test_pipeline_signal_connect_null_pipeline)
{
  GstcStatus ret;
  const gchar *pipeline_name = NULL;
  const gchar *element_name = "element_name";
  const gchar *signal_name = "signal_name";
  gint signal_timeout = -1;
  gchar *response;

  ret = gstc_pipeline_signal_connect (_client, pipeline_name, element_name,
      signal_name, signal_timeout, &response);
  assert_equals_int (GSTC_NULL_ARGUMENT, ret);
}

GST_END_TEST;

GST_START_TEST (test_pipeline_signal_connect_null_element)
{
  GstcStatus ret;
  const gchar *pipeline_name = "pipe";
  const gchar *element_name = NULL;
  const gchar *signal_name = "signal_name";
  gint signal_timeout = -1;
  gchar *response;

  ret = gstc_pipeline_signal_connect (_client, pipeline_name, element_name,
      signal_name, signal_timeout, &response);
  assert_equals_int (GSTC_NULL_ARGUMENT, ret);
}

GST_END_TEST;

GST_START_TEST (test_pipeline_signal_connect_null_signal)
{
  GstcStatus ret;
  const gchar *pipeline_name = "pipe";
  const gchar *element_name = "element_name";
  const gchar *signal_name = NULL;
  gint signal_timeout = -1;
  gchar *response;

  ret = gstc_pipeline_signal_connect (_client, pipeline_name, element_name,
      signal_name, signal_timeout, &response);
  assert_equals_int (GSTC_NULL_ARGUMENT, ret);
}

GST_END_TEST;

GST_START_TEST (test_pipeline_signal_connect_null_client)
{
  GstcStatus ret;
  const gchar *pipeline_name = "pipe";
  const gchar *element_name = "element_name";
  const gchar *signal_name = "signal_name";
  gint signal_timeout = -1;
  gchar *response;

  ret = gstc_pipeline_signal_connect (NULL, pipeline_name, element_name,
      signal_name, signal_timeout, &response);
  assert_equals_int (GSTC_NULL_ARGUMENT, ret);
}

GST_END_TEST;

static Suite *
libgstc_pipeline_suite (void)
{
  Suite *suite = suite_create ("libgstc_pipeline");
  TCase *tc = tcase_create ("general");

  suite_add_tcase (suite, tc);

  tcase_add_checked_fixture (tc, setup, teardown);
  tcase_add_test (tc, test_pipeline_signal_connect_success);
  tcase_add_test (tc, test_pipeline_signal_connect_null_pipeline);
  tcase_add_test (tc, test_pipeline_signal_connect_null_element);
  tcase_add_test (tc, test_pipeline_signal_connect_null_signal);
  tcase_add_test (tc, test_pipeline_signal_connect_null_client);

  return suite;
}

GST_CHECK_MAIN (libgstc_pipeline);
