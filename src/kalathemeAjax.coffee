###*
* @file
* Overrides for core AJAX functionality.
* See misc/ajax.js
*
* Thanks bootstrap theme for insperation
* @link https://drupal.org/project/bootstrap
###
(($) ->
  #Make sure we can attach this to an object.
  window.Drupal.ajax ?= ->

  Drupal.ajax::beforeSend = (xmlhttprequest, options) ->
    # For forms without file inputs, the jQuery Form plugin serializes the form
    # values, and then calls jQuery's $.ajax() function, which invokes this
    # handler. In this circumstance, options.extraData is never used. For forms
    # with file inputs, the jQuery Form plugin uses the browser's normal form
    # submission mechanism, but captures the response in a hidden IFRAME. In this
    # circumstance, it calls this handler first, and then appends hidden fields
    # to the form to submit the values in options.extraData. There is no simple
    # way to know which submission mechanism will be used, so we add to extraData
    # regardless, and allow it to be ignored in the former case.
    if @form
      options.extraData = options.extraData or {}

      # Let the server know when the IFRAME submission mechanism is used. The
      # server can use this information to wrap the JSON response in a TEXTAREA,
      # as per http://jquery.malsup.com/form/#file-upload.
      options.extraData.ajax_iframe_upload = "1"

      # The triggering element is about to be disabled (see below), but if it
      # contains a value (e.g., a checkbox, textfield, select, etc.), ensure that
      # value is included in the submission. As per above, submissions that use
      # $.ajax() are already serialized prior to the element being disabled, so
      # this is only needed for IFRAME submissions.
      v = $.fieldValue(@element)
      options.extraData[@element.name] = v  if v isnt null

    # Disable the element that received the change to prevent user interface
    # interaction while the Ajax request is in progress. ajax.ajaxing prevents
    # the element from triggering a new request, but does not prevent the user
    # from changing its value.
    $(@element).addClass("progress-disabled").attr "disabled", true

    # Insert progressbar or throbber.
    if @progress.type is "bar"
      progressBar = new Drupal.progressBar("ajax-progress-" + @element.id, eval_(@progress.update_callback), @progress.method, eval_(@progress.error_callback))
      progressBar.setProgress -1, @progress.message  if @progress.message
      progressBar.startMonitoring @progress.url, @progress.interval or 1500  if @progress.url
      @progress.element = $(progressBar.element).addClass("ajax-progress ajax-progress-bar")
      @progress.object = progressBar
      $(@element).after @progress.element
    else if @progress.type is "throbber"
      iconClasses = if Drupal.settings.kalatheme.fontawesome is true then "fa fa-refresh fa-spin" else "glyphicon glyphicon-refresh glyphicon-spin"
      markup = "<div class=\"ajax-progress ajax-progress-throbber\">"
      markup += "<span class=\"#{iconClasses}\" aria-hidden=\"true\"></span><span class=\"sr-only\">Loading</span></div>"
      @progress.element = $(markup)
      # If element is an input type, append after.
      if $(@element).is("input")
        $(".throbber", @progress.element).after "<div class=\"message\">" + @progress.message + "</div>"  if @progress.message
        $(@element).after @progress.element

      # Otherwise inject it inside the element.
      else
        $(".throbber", @progress.element).append "<div class=\"message\">" + @progress.message + "</div>"  if @progress.message
        $(@element).append @progress.element
)(jQuery)
