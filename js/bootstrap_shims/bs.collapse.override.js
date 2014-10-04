(function($){

    $.fn.collapse.Constructor.prototype.hide = function () {

      if (this.transitioning || !this.$element.hasClass('in')) return

      var startEvent = $.Event('hide.bs.collapse')
      this.$element.trigger(startEvent)
      if (startEvent.isDefaultPrevented()) return

      var dimension = this.dimension()

      this.$element[dimension](this.$element[dimension]())[0].offsetHeight

      this.$element
        .addClass('collapsing')
        .removeClass('collapse')
        .removeClass('in')

      this.transitioning = 1
      var complete = function () {
        this.transitioning = 0
        this.$element
          .trigger('hidden.bs.collapse')
          .removeClass('collapsing')
          .removeClass('in')
          .addClass('collapse')
      }

      if (!$.support.transition) return complete.call(this)

      this.$element
        [dimension](0)
        .one('bsTransitionEnd', $.proxy(complete, this))
        .emulateTransitionEnd(350)
    }

}(jQuery));
