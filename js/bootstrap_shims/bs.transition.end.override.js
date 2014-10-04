(function($){
  // http://blog.alexmaccaw.com/css-transitions
  $.fn.emulateTransitionEnd = function (duration) {
    var called = false;
    var $el = this;
    var that = this;
    $(this).on('bsTransitionEnd', function () {
      called = true;
      $(that).off('bsTransitionEnd');
    })
    var callback = function () { if (!called) $($el).trigger($.support.transition.end) }
    setTimeout(callback, duration)
    return this
  }
}(jQuery));
