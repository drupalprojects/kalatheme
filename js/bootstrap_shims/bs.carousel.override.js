(function($){

  $.fn.carousel.Constructor.prototype.slide = function (type, next) {
    var $active   = this.$element.find('.item.active')
    var $next     = next || $active[type]()
    var isCycling = this.interval
    var direction = type == 'next' ? 'left' : 'right'
    var fallback  = type == 'next' ? 'first' : 'last'
    var that      = this

    if (!$next.length) {
      if (!this.options.wrap) return
      $next = this.$element.find('.item')[fallback]()
    }
    if ($next.hasClass('active')) return (this.sliding = false)


    var relatedTarget = $next[0]
    var slideEvent = $.Event('slide.bs.carousel', {
      relatedTarget: relatedTarget,
      direction: direction
    })
    this.$element.trigger(slideEvent)
    if (slideEvent.isDefaultPrevented()) return

    this.sliding = true

    isCycling && this.pause()

    if (this.$indicators.length) {
      // clear them all
      this.$indicators.find('.active').removeClass('active')
      var $nextIndicator = $(this.$indicators.children()[this.getItemIndex($next)])
      $nextIndicator.addClass('active')
    }

    var slidEvent = $.Event('slid.bs.carousel', { relatedTarget: relatedTarget, direction: direction }) // yes, "slid"
    if ($.support.transition && this.$element.hasClass('slide')) {
      $next.addClass(type)
      $next[0].offsetWidth // force reflow
      $active.addClass(direction)
      $next.addClass(direction)
      $active.on('bsTransitionEnd', this.onSlideComplete.bind(this,[$next, $active, direction, type, slideEvent]))
        .emulateTransitionEnd($active.css('transition-duration').slice(0, -1) * 1000 - 150);
    } else {
      $active.removeClass('active')
      $next.addClass('active')
      this.sliding = false
      this.$element.trigger(slidEvent)
    }

    isCycling && this.cycle()

    return this;
  }

  $.fn.carousel.Constructor.prototype.onSlideComplete = function( vars ){
    this.sliding = false;
      var $next = vars[0];
      var $active = vars[1];
      var direction = vars[2];
      var type = vars[3];
      var slidEvent = vars[4];
      var that = this;
      $active.off('bsTransitionEnd')
      $next.removeClass(type);
      $next.removeClass(direction);
      $next.addClass('active');
      $active.removeClass('active');
      $active.removeClass(direction);
      this.$element.find(".carousel-inner").css({
        'max-height': $next.height()+"px",
        'height': $next.height()+"px"
      });
      setTimeout(function () {
        // console.log("trigger slid event!");
        that.$element.trigger(slidEvent);
      }, 0);
      return this;
  }


}(jQuery));
