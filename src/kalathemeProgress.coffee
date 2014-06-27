###*
* @file
* Overrides for Progressbar function
* See misc/batch.js
* Thanks to @link https://drupal.org/project/bootstrap
###
(($) ->
  window.Drupal ?= {}
  ###
  A progressbar object. Initialized with the given id. Must be inserted into
  the DOM afterwards through progressBar.element.

  method is the function which will perform the HTTP request to get the
  progress bar state. Either "GET" or "POST".

  e.g. pb = new progressBar('myProgressBar');
  some_element.appendChild(pb.element);
  ###
  class ProgressBar
    pb = @
    constructor: (@id, @updateCallback, @method, @errorCallback) ->
      el = $("<div class=\"progress-wrapper\" aria-live=\"polite\"></div>")
      # The WAI-ARIA setting aria-live="polite" will announce changes after users
      # have completed their current activity and not interrupt the screen reader.
      modalHtml = "<div id =\"#{@id}\" class=\"progress progress-striped active\""
      modalHtml += "aria-describedby=\"message#{@id}\">\n<div class=\"progress-bar\""
      modalHtml += " role=\"progressbar\" aria-valuemin=\"0\" aria-valuemax=\"100\" "
      modalHtml += "aria-valuenow=\"0\">\n<div class=\"percentage\">\n</div>\n</div>\n</div>"
      modalHtml += "</div>\n"
      modalHtml += "<p class=\"message\ help-block\" id=\"message#{@id}\"></p>"
      el.html(modalHtml)
      @element = el

    ###
    Set the percentage and status message for the progressbar.
    ###
    setProgress: (percentage, message) ->
      if percentage >= 0 and percentage <= 100
        $(".progress-bar", @element).css "width", percentage + "%"
        $(".progress-bar", @element).attr "aria-valuenow", percentage
        $(".percentage", @element).html percentage + "%"

      $(".message", @element).html( message )
      if this.updateCallback
        @updateCallback( percentage, message, this )

    ###
    Start monitoring progress via Ajax.
    ###
    startMonitoring: (uri, delay) ->
      @delay = delay
      @uri = uri
      @sendPing()


    ###
    Stop monitoring progress via Ajax.
    ###
    stopMonitoring:  ->
      clearTimeout @timer

      # This allows monitoring to be stopped from within the callback.
      @uri = null


    ###
    Request progress data from server.
    ###
    sendPing: () ->
      clearTimeout @timer  if @timer
      if @uri
        pb = this

        # When doing a post request, you need non-null data. Otherwise a
        # HTTP 411 or HTTP 406 (with Apache mod_security) error may result.
        $.ajax
          type: @method
          url: @uri
          data: ""
          dataType: "json"
          success: (progress) ->

            # Display errors.
            if progress.status is 0
              pb.displayError progress.data


            # Update display.
            pb.setProgress progress.percentage, progress.message

            # Schedule next timer.
            pb.timer = setTimeout(->
              pb.sendPing()
            , pb.delay)

          error: (xmlhttp) ->
            pb.displayError Drupal.ajaxError(xmlhttp, pb.uri)

    ###
    Display errors on the page.
    ###
    displayError: (string) ->
      errorHtml = "<div class=\"alert alert-block alert-error\" role=\"alert\">"
      errorHtml += "<button type=\"button\" class=\"close\""
      errorHtml += " data-dismiss=\"alert\">"
      errorHtml += "<span aria-hidden=\"true\">&times;</span>"
      errorHtml += "     <span class=\"sr-only\">Close</span></button>"
      errorHtml += "<h4>Error message</h4></div>"
      error = $(errorHtml).append(string)
      $(@element).before(error).hide()
      @errorCallback this  if @errorCallback

  Drupal.progressBar = ProgressBar

) jQuery
