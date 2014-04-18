(($) ->
  
  ###
  A progressbar object. Initialized with the given id. Must be inserted into
  the DOM afterwards through progressBar.element.
  
  method is the function which will perform the HTTP request to get the
  progress bar state. Either "GET" or "POST".
  
  e.g. pb = new progressBar('myProgressBar');
  some_element.appendChild(pb.element);
  ###
  Drupal.progressBar = (id, updateCallback, method, errorCallback) ->
    pb = this
    @id = id
    @method = method or "GET"
    @updateCallback = updateCallback
    @errorCallback = errorCallback
    
    # The WAI-ARIA setting aria-live="polite" will announce changes after users
    # have completed their current activity and not interrupt the screen reader.
    @element = $("<div class=\"progress-wrapper\" aria-live=\"polite\"></div>")
    @element.html "<div id =\"" + id + "\" class=\"progress progress-striped active\" aria-describedby=\"message" + id + "\">" + "<div class=\"progress-bar\" role=\"progressbar\" aria-valuemin=\"0\" aria-valuemax=\"100\" aria-valuenow=\"0\">" + "<div class=\"percentage\"></div>" + "</div></div>" + "</div><div class=\"percentage pull-right\"></div>" + "<div class=\"message\" id=\"message" + id + "\">&nbsp;</div>"
    return

  
  ###
  Set the percentage and status message for the progressbar.
  ###
  Drupal.progressBar::setProgress = (percentage, message) ->
    if percentage >= 0 and percentage <= 100
      $("div.progress-bar", @element).css "width", percentage + "%"
      $("div.progress-bar", @element).attr "aria-valuenow", percentage
      $("div.percentage", @element).html percentage + "%"
    $("div.message", @element).html message
    @updateCallback percentage, message, this  if @updateCallback
    return

  
  ###
  Start monitoring progress via Ajax.
  ###
  Drupal.progressBar::startMonitoring = (uri, delay) ->
    @delay = delay
    @uri = uri
    @sendPing()
    return

  
  ###
  Stop monitoring progress via Ajax.
  ###
  Drupal.progressBar::stopMonitoring = ->
    clearTimeout @timer
    
    # This allows monitoring to be stopped from within the callback.
    @uri = null
    return

  
  ###
  Request progress data from server.
  ###
  Drupal.progressBar::sendPing = ->
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
            return
          
          # Update display.
          pb.setProgress progress.percentage, progress.message
          
          # Schedule next timer.
          pb.timer = setTimeout(->
            pb.sendPing()
            return
          , pb.delay)
          return

        error: (xmlhttp) ->
          pb.displayError Drupal.ajaxError(xmlhttp, pb.uri)
          return

    return

  
  ###
  Display errors on the page.
  ###
  Drupal.progressBar::displayError = (string) ->
    error = $("<div class=\"alert alert-block alert-error\" role=\"alert\"><button type=\"button\" class=\"close\" data-dismiss=\"alert\">&times;</button><h4>Error message</h4></div>").append(string)
    $(@element).before(error).hide()
    @errorCallback this  if @errorCallback
    return

  return
) jQuery
