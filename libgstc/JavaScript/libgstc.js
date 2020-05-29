// GStreamer Daemon - gst-launch on steroids
// JavaScript client library abstracting gstd interprocess communication

// Copyright (c) 2015-2020 RidgeRun, LLC (http://www.ridgerun.com)

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:

// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials provided
// with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

/*
GstClient - GstdClient Class
*/
class GstdClient {

  constructor(ip='http://localhost',port=5000){
    this.ip = ip;
    this.port = port;
  }

  static send_cmd(http, callback) {
    // Check if the fetch operation is complete
    if (http.readyState === XMLHttpRequest.DONE) {
      // Check if the data transfer has been completed successfully
      var status = http.status;
      // Check for 'HTTP response status codes'
      // Handling Client errors (200–499) internally
      if (status === 0 || (status >= 200 && status < 500)) {
        var gstdResponse = JSON.parse(http.responseText);
        if (gstdResponse.code === GstcErrorCode.GSTC_OK) {
          callback(gstdResponse);
        } else {
          throw new GstdError([gstdResponse.description, gstdResponse.code]);
        }
      } else {
        throw new GstcError(['Server did not respond. Is it up?',
          GstcErrorCode.GSTC_UNREACHABLE]);
      }
    }
  }

  static check_callback(callback) {
    if (typeof callback !== "function"){
      console.error ("Provide a callback function");
      return false;
    }
    return true;
  }

  list_pipelines(callback) {

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('GET', this.ip + ":" + this.port + "/pipelines");
    http.send();
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }

  pipeline_create(pipe_name, pipe_desc, callback){

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('POST', this.ip + ":" + this.port + "/pipelines?name="+pipe_name+"&description="+pipe_desc);
    var jBodyMsg = JSON.stringify({
      name: pipe_name,
      description: pipe_desc
    });
    http.send(jBodyMsg);
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }

  pipeline_play(pipe_name, callback){

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('PUT', this.ip + ":" + this.port + "/pipelines/"+pipe_name+"/state?name=playing");
    var jBodyMsg = JSON.stringify({
      name: "playing"
    });
    http.send(jBodyMsg);
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }

  element_set(pipe_name, element, prop, value, callback){

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('PUT', this.ip + ":" + this.port + "/pipelines/"+pipe_name+"/elements/"+element+"/properties/"+prop+"?name="+value);
    var jBodyMsg = JSON.stringify({
      name: "paused"
    });
    http.send(jBodyMsg);
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }

  pipeline_pause(pipe_name, callback){

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('PUT', this.ip + ":" + this.port + "/pipelines/"+pipe_name+"/state?name=paused");
    var jBodyMsg = JSON.stringify({
      name: "paused"
    });
    http.send(jBodyMsg);
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }

  pipeline_stop(pipe_name, callback){

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('PUT', this.ip + ":" + this.port + "/pipelines/"+pipe_name+"/state?name=null");
    var jBodyMsg = JSON.stringify({
      name: "null"
    });
    http.send(jBodyMsg);
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }

  pipeline_delete(pipe_name, callback){

    if(!GstdClient.check_callback(callback)) {
      return TypeError;
    }

    var http = new XMLHttpRequest();
    http.open('DELETE', this.ip + ":" + this.port + "/pipelines?name="+pipe_name);
    var jBodyMsg = JSON.stringify({
      name: pipe_name
    });
    http.send(jBodyMsg);
    http.onreadystatechange = function (){
      GstdClient.send_cmd(http, callback);
    }
  }
}

/*
GstClient - GstcError Class
*/
class GstcError extends Error {
  constructor(...params) {
    // Pass remaining arguments (including vendor specific ones) to parent constructor
    super(...params)

    // Maintains proper stack trace for where our error was thrown (only available on V8)
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, GstcError)
    }

    this.name = 'GstcError'
    // Custom debugging information
    this.date = new Date()
  }
}

/*
GstClient - GstdError Class
*/
class GstdError extends Error {
  constructor(...params) {
    // Pass remaining arguments (including vendor specific ones) to parent constructor
    super(...params)

    // Maintains proper stack trace for where our error was thrown (only available on V8)
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, GstdError)
    }

    this.name = 'GstdError'
    // Custom debugging information
    this.date = new Date()
  }
}

const GstcErrorCode = {
  GSTC_OK: 0,
  GSTC_NULL_ARGUMENT: -1,
  GSTC_UNREACHABLE: -2,
  GSTC_TIMEOUT: -3,
  GSTC_OOM: -4,
  GSTC_TYPE_ERROR: -5,
  GSTC_MALFORMED: -6,
  GSTC_NOT_FOUND: -7,
  GSTC_SEND_ERROR: -8,
  GSTC_RECV_ERROR: -9,
  GSTC_SOCKET_ERROR: -10,
  GSTC_THREAD_ERROR: -11,
  GSTC_BUS_TIMEOUT: -12,
  GSTC_SOCKET_TIMEOUT: -13,
}