/*! kalatheme - v3.0.0+dev - 2014-05-02
* https://drupal.org/project/kalatheme
* Copyright (c) 2014 ; Licensed  *//* ========================================================================
* Extends Bootstrap v3.1.1

* Copyright (c) <2014> eBay Software Foundation

* All rights reserved.

* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

* Neither the name of eBay or any of its subsidiaries or affiliates nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* ======================================================================== */

(function($) {
  "use strict";

  var uniqueId = function(prefix) {
      return (prefix || 'ui-id') + '-' + Math.floor((Math.random()*1000)+1)
  }

  // Alert Extension
  // ===============================

    $('.alert').attr('role', 'alert')
    $('.close').removeAttr('aria-hidden').wrapInner('<span aria-hidden="true"></span>').append('<span class="sr-only">Close</span>')

  // TOOLTIP Extension
  // ===============================

    var showTooltip =    $.fn.tooltip.Constructor.prototype.show
        , hideTooltip =    $.fn.tooltip.Constructor.prototype.hide

    $.fn.tooltip.Constructor.prototype.show = function () {
        showTooltip.apply(this, arguments)
        var $tip = this.tip()
            , tooltipID = $tip.attr('id') || uniqueId('ui-tooltip')
        $tip.attr({'role':'tooltip','id' : tooltipID})
        this.$element.attr('aria-describedby', tooltipID)
    }

    $.fn.tooltip.Constructor.prototype.hide = function () {
        hideTooltip.apply(this, arguments)
        removeMultiValAttributes(this.$element, 'aria-describedby', this.tip().attr('id'))
        return this
    }

  // Popover Extension
  // ===============================
    var showPopover =   $.fn.popover.Constructor.prototype.setContent
      , hideTPopover =   $.fn.popover.Constructor.prototype.hide

    $.fn.popover.Constructor.prototype.setContent = function(){
      showPopover.apply(this, arguments)
      var $tip = this.tip()
        , tooltipID = $tip.attr('id') || uniqueId('ui-tooltip')
      $tip.attr({'role':'alert','id' : tooltipID})
      this.$element.attr('aria-describedby', tooltipID)
      this.$element.focus()
    }
    $.fn.popover.Constructor.prototype.hide =  function(){
        hideTooltip.apply(this, arguments)
        removeMultiValAttributes(this.$element, 'aria-describedby', this.tip().attr('id'))
    }

  //Modal Extension
    $('.modal-dialog').attr( {'role' : 'document'})
    var modalhide =   $.fn.modal.Constructor.prototype.hide
    $.fn.modal.Constructor.prototype.hide = function(){
       var modalOpener = this.$element.parent().find('[data-target="#' + this.$element.attr('id') + '"]')
       modalhide.apply(this, arguments)
       modalOpener.focus()
    }

  // DROPDOWN Extension
  // ===============================

    var toggle   = '[data-toggle=dropdown]'
      , $par
      , firstItem
      , focusDelay = 200
      , menus = $(toggle).parent().find('ul').attr('role','menu')
      , lis = menus.find('li').attr('role','presentation')

    lis.find('a').attr({'role':'menuitem', 'tabIndex':'-1'})
    $(toggle).attr({ 'aria-haspopup':'true', 'aria-expanded': 'false'})

    $(toggle).parent().on('shown.bs.dropdown',function(e){
      $par = $(this)
      var $toggle = $par.find(toggle)
      $toggle.attr('aria-expanded','true')

      setTimeout(function(){
            firstItem = $('.dropdown-menu [role=menuitem]:visible', $par)[0]
            try{ firstItem.focus()} catch(ex) {}
      }, focusDelay)
    })

    $(toggle).parent().on('hidden.bs.dropdown',function(e){
      $par = $(this)
      var $toggle = $par.find(toggle)
      $toggle.attr('aria-expanded','false')
    })

    //Adding Space Key Behaviour, opens on spacebar
    $.fn.dropdown.Constructor.prototype.keydown = function (e) {
      var  $par
        , firstItem
      if (!/(32)/.test(e.keyCode)) return
        $par = $(this).parent()
        $(this).trigger ("click")
        e.preventDefault() && e.stopPropagation()
    }

    $(document)
      .on('focusout.dropdown.data-api', '.dropdown-menu', function(e){
        var $this = $(this)
                    , that = this
        setTimeout(function() {
         if(!$.contains(that, document.activeElement)){
          $this.parent().removeClass('open')
          $this.parent().find('[data-toggle=dropdown]').attr('aria-expanded','false')
         }
        }, 150)
       })
      .on('keydown.bs.dropdown.data-api', toggle + ', [role=menu]' , $.fn.dropdown.Constructor.prototype.keydown)


  // Tab Extension
  // ===============================

    var $tablist = $('.nav-tabs')
        , $lis = $tablist.children('li')
        , $tabs = $tablist.find('[data-toggle="tab"], [data-toggle="pill"]')

    $tablist.attr('role', 'tablist')
    $lis.attr('role', 'presentation')
    $tabs.attr('role', 'tab')

    $tabs.each(function( index ) {
      var tabpanel = $($(this).attr('href'))
        , tab = $(this)
        , tabid = tab.attr('id') || uniqueId('ui-tab')

        tab.attr('id', tabid)

      if(tab.parent().hasClass('active')){
        tab.attr( { 'tabIndex' : '0', 'aria-expanded' : 'true', 'aria-selected' : 'true', 'aria-controls': tab.attr('href').substr(1) } )
        tabpanel.attr({ 'role' : 'tabpanel', 'tabIndex' : '0', 'aria-hidden' : 'false', 'aria-labelledby':tabid })
      }else{
        tab.attr( { 'tabIndex' : '-1', 'aria-expanded' : 'false', 'aria-selected' : 'false', 'aria-controls': tab.attr('href').substr(1) } )
        tabpanel.attr( { 'role' : 'tabpanel', 'tabIndex' : '-1', 'aria-hidden' : 'true', 'aria-labelledby':tabid } )
      }
    })

    $.fn.tab.Constructor.prototype.keydown = function (e) {
      var $this = $(this)
      , $items
      , $ul = $this.closest('ul[role=tablist] ')
      , index
      , k = e.which || e.keyCode

      $this = $(this)
      if (!/(37|38|39|40)/.test(k)) return

      $items = $ul.find('[role=tab]:visible')
      index = $items.index($items.filter(':focus'))

      if (k == 38 || k == 37) index--                         // up & left
      if (k == 39 || k == 40) index++                        // down & right


      if(index < 0) index = $items.length -1
      if(index == $items.length) index = 0

      var nextTab = $items.eq(index)
      if(nextTab.attr('role') ==='tab'){

        nextTab.tab('show')      //Comment this line for dynamically loaded tabPabels, to save Ajax requests on arrow key navigation
        .focus()
      }
      // nextTab.focus()

      e.preventDefault()
      e.stopPropagation()
    }

    $(document).on('keydown.tab.data-api','[data-toggle="tab"], [data-toggle="pill"]' , $.fn.tab.Constructor.prototype.keydown)

   var tabactivate =    $.fn.tab.Constructor.prototype.activate;
   $.fn.tab.Constructor.prototype.activate = function (element, container, callback) {
      var $active = container.find('> .active')
      $active.find('[data-toggle=tab]').attr({ 'tabIndex' : '-1','aria-selected' : false,'aria-expanded' : false })
      $active.filter('.tab-pane').attr({ 'aria-hidden' : true,'tabIndex' : '-1' })

      tabactivate.apply(this, arguments)

      element.addClass('active')
      element.find('[data-toggle=tab]').attr({ 'tabIndex' : '0','aria-selected' : true,'aria-expanded' : true })
      element.filter('.tab-pane').attr({ 'aria-hidden' : false,'tabIndex' : '0' })
   }


  // Collapse Extension
  // ===============================

      var $colltabs =  $('[data-toggle="collapse"]')
      $colltabs.attr({ 'role':'tab', 'aria-selected':'false', 'aria-expanded':'false' })
      $colltabs.each(function( index ) {
        var colltab = $(this)
        , collpanel = (colltab.attr('data-target')) ? $(colltab.attr('data-target')) : $(colltab.attr('href'))
        , parent  = colltab.attr('data-parent')
        , collparent = parent && $(parent)
        , collid = colltab.attr('id') || uniqueId('ui-collapse')

        $(collparent).find('div:not(.collapse,.panel-body), h4').attr('role','presentation')

          colltab.attr('id', collid)
          if(collparent){
            collparent.attr({ 'role' : 'tablist', 'aria-multiselectable' : 'true' })
            if(collpanel.hasClass('in')){
              colltab.attr({ 'aria-controls': colltab.attr('href').substr(1), 'aria-selected':'true', 'aria-expanded':'true', 'tabindex':'0' })
              collpanel.attr({ 'role':'tabpanel', 'tabindex':'0', 'aria-labelledby':collid, 'aria-hidden':'false' })
            }else{
              colltab.attr({'aria-controls' : colltab.attr('href').substr(1), 'tabindex':'-1' })
              collpanel.attr({ 'role':'tabpanel', 'tabindex':'-1', 'aria-labelledby':collid, 'aria-hidden':'true' })
            }
          }
      })

    var collToggle = $.fn.collapse.Constructor.prototype.toggle
    $.fn.collapse.Constructor.prototype.toggle = function(){
        var prevTab = this.$parent && this.$parent.find('[aria-expanded="true"]') , href

        if(prevTab){
          var prevPanel = prevTab.attr('data-target') || (href = prevTab.attr('href')) && href.replace(/.*(?=#[^\s]+$)/, '')
          , $prevPanel = $(prevPanel)
          , $curPanel = this.$element
          , par = this.$parent
          , curTab

        if (this.$parent) curTab = this.$parent.find('[data-toggle=collapse][href="#' + this.$element.attr('id') + '"]')

        collToggle.apply(this, arguments)

        if ($.support.transition) {
          this.$element.one($.support.transition.end, function(){

              prevTab.attr({ 'aria-selected':'false','aria-expanded':'false', 'tabIndex':'-1' })
              $prevPanel.attr({ 'aria-hidden' : 'true','tabIndex' : '-1'})

              curTab.attr({ 'aria-selected':'true','aria-expanded':'true', 'tabIndex':'0' })

              if($curPanel.hasClass('in')){
                $curPanel.attr({ 'aria-hidden' : 'false','tabIndex' : '0' })
              }else{
                curTab.attr({ 'aria-selected':'false','aria-expanded':'false'})
                $curPanel.attr({ 'aria-hidden' : 'true','tabIndex' : '-1' })
              }
          })
        }
      }else{
        collToggle.apply(this, arguments)
      }
    }

    $.fn.collapse.Constructor.prototype.keydown = function (e) {
      var $this = $(this)
      , $items
      , $tablist = $this.closest('div[role=tablist] ')
      , index
      , k = e.which || e.keyCode

      $this = $(this)
      if (!/(32|37|38|39|40)/.test(k)) return
      if(k==32) $this.click()

      $items = $tablist.find('[role=tab]')
      index = $items.index($items.filter(':focus'))

      if (k == 38 || k == 37) index--                                        // up & left
      if (k == 39 || k == 40) index++                        // down & right
      if(index < 0) index = $items.length -1
      if(index == $items.length) index = 0

      $items.eq(index).focus()

      e.preventDefault()
      e.stopPropagation()

    }

    $(document).on('keydown.collapse.data-api','[data-toggle="collapse"]' ,  $.fn.collapse.Constructor.prototype.keydown)

  // Carousel Extension
  // ===============================

      $('.carousel').each(function (index) {
        var $this = $(this)
          , prev = $this.find('[data-slide="prev"]')
          , next = $this.find('[data-slide="next"]')
          , $options = $this.find('.item')
          , $listbox = $options.parent()

        $this.attr( { 'data-interval' : 'false', 'data-wrap' : 'false' } )
        $listbox.attr('role', 'listbox')
        $options.attr('role', 'option')

        var spanPrev = document.createElement('span')
        spanPrev.setAttribute('class', 'sr-only')
        spanPrev.innerHTML='Previous'

        var spanNext = document.createElement('span')
        spanNext.setAttribute('class', 'sr-only')
        spanNext.innerHTML='Next'

        prev.attr('role', 'button')
        next.attr('role', 'button')

        prev.append(spanPrev)
        next.append(spanNext)

        $options.each(function () {
          var item = $(this)
          if(item.hasClass('active')){
            item.attr({ 'aria-selected': 'true', 'tabindex' : '0' })
          }else{
            item.attr({ 'aria-selected': 'false', 'tabindex' : '-1' })
          }
        })
      })

      var slideCarousel = $.fn.carousel.Constructor.prototype.slide
      $.fn.carousel.Constructor.prototype.slide = function (type, next) {
        var $active = this.$element.find('.item.active')
          , $next = next || $active[type]()

        slideCarousel.apply(this, arguments)

      $active
        .one($.support.transition.end, function () {
        $active.attr({'aria-selected':false, 'tabIndex': '-1'})
        $next.attr({'aria-selected':true, 'tabIndex': '0'})
        //.focus()
       })
      }

    $.fn.carousel.Constructor.prototype.keydown = function (e) {
     var $this = $(this)
      , $ul = $this.closest('div[role=listbox]')
      , $items = $ul.find('[role=option]')
      , $parent = $ul.parent()
      , k = e.which || e.keyCode
      , index
      , i

      if (!/(37|38|39|40)/.test(k)) return

      index = $items.index($items.filter('.active'))
      if (k == 37 || k == 38) {                           //  Up
        $parent.carousel('prev')
        index--
        if(index < 0) index = $items.length -1
        else  $this.prev().focus()

      }
      if (k == 39 || k == 40) {                          // Down
        $parent.carousel('next')
        index++
        if(index == $items.length) index = 0
        else  {
          $this.one($.support.transition.end, function () {
            $this.next().focus()
          })
        }

      }

      e.preventDefault()
      e.stopPropagation()
    }
    $(document).on('keydown.carousel.data-api', 'div[role=option]', $.fn.carousel.Constructor.prototype.keydown)

  // GENERAL UTILITY FUNCTIONS
  // ===============================

    var removeMultiValAttributes = function (el, attr, val) {
     var describedby = (el.attr( attr ) || "").split( /\s+/ )
        , index = $.inArray(val, describedby)
     if ( index !== -1 ) {
       describedby.splice( index, 1 )
     }
     describedby = $.trim( describedby.join( " " ) )
     if (describedby ) {
       el.attr( attr, describedby )
     } else {
      el.removeAttr( attr )
     }
    }


})(jQuery);


/**
* @file
* Overrides for CTools modal.
* See ctools/js/modal.js
 */

(function() {
  (function($) {

    /*
    Override CTools modal show function so it can recognize
    the Bootstrap modal classes correctly
     */
    if (window.Drupal == null) {
      window.Drupal = {};
    }
    if (Drupal.CTools == null) {
      Drupal.CTools = {
        Modal: {
          show: null
        }
      };
    }
    Drupal.CTools.Modal.show = function(choice) {
      var defaults, opts, resize, settings;
      opts = {};
      if (choice && typeof choice === "string" && Drupal.settings[choice]) {
        $.extend(true, opts, Drupal.settings[choice]);
      } else {
        if (choice) {
          $.extend(true, opts, choice);
        }
      }
      defaults = {
        modalTheme: "CToolsModalDialog",
        throbberTheme: "CToolsModalThrobber",
        animation: "show",
        animationSpeed: "fast",
        modalSize: {
          type: "scale",
          width: 0.8,
          height: 0.8,
          addWidth: 0,
          addHeight: 0,
          contentRight: 25,
          contentBottom: 45
        },
        modalOptions: {
          opacity: 0.55,
          background: "#fff"
        }
      };
      settings = {};
      $.extend(true, settings, defaults, Drupal.settings.CToolsModal, opts);
      if (Drupal.CTools.Modal.currentSettings && Drupal.CTools.Modal.currentSettings !== settings) {
        Drupal.CTools.Modal.modal.remove();
        Drupal.CTools.Modal.modal = null;
      }
      Drupal.CTools.Modal.currentSettings = settings;
      resize = function(e) {
        var context, height, width;
        context = (e ? document : Drupal.CTools.Modal.modal);
        if (Drupal.CTools.Modal.currentSettings.modalSize.type === "scale") {
          width = $(window).width() * Drupal.CTools.Modal.currentSettings.modalSize.width;
          height = $(window).height() * Drupal.CTools.Modal.currentSettings.modalSize.height;
        } else {
          width = Drupal.CTools.Modal.currentSettings.modalSize.width;
          height = Drupal.CTools.Modal.currentSettings.modalSize.height;
        }
        $("div.ctools-modal-dialog", context).css({
          width: width + Drupal.CTools.Modal.currentSettings.modalSize.addWidth + "px",
          height: height + Drupal.CTools.Modal.currentSettings.modalSize.addHeight + "px"
        });
        $("div.ctools-modal-dialog .modal-body", context).css({
          width: (width - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + "px",
          height: (height - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + "px"
        });
      };
      if (!Drupal.CTools.Modal.modal) {
        Drupal.CTools.Modal.modal = $(Drupal.theme(settings.modalTheme));
        if (settings.modalSize.type === "scale") {
          $(window).bind("resize", resize);
        }
      }
      $("body").addClass("modal-open");
      resize();
      $(".modal-title", Drupal.CTools.Modal.modal).html(Drupal.CTools.Modal.currentSettings.loadingText);
      Drupal.CTools.Modal.modalContent(Drupal.CTools.Modal.modal, settings.modalOptions, settings.animation, settings.animationSpeed);
      $("#modalContent .modal-body").html(Drupal.theme(settings.throbberTheme));
    };
    Drupal.CTools.Modal.dismiss = function() {
      console.log("oi");
      if (Drupal.CTools.Modal.modal) {
        $("body").removeClass("modal-open");
        Drupal.CTools.Modal.unmodalContent(Drupal.CTools.Modal.modal);
      }
    };
    if (Drupal.theme == null) {
      Drupal.theme = function() {};
    }

    /*
    Provide the HTML for the Modal.
     */
    Drupal.theme.prototype.CToolsModalDialog = function() {
      var html;
      html = "";
      html += "  <div id=\"ctools-modal\">";
      html += "    <div class=\"ctools-modal-dialog modal-dialog\">";
      html += "      <div class=\"modal-content\">";
      html += "        <div class=\"modal-header\">";
      html += "          <button type=\"button\" class=\"close ctools-close-modal\" aria-hidden=\"true\">&times;</button>";
      html += "          <h4 id=\"modal-title\" class=\"modal-title\">&nbsp;</h4>";
      html += "        </div>";
      html += "        <div id=\"modal-content\" class=\"modal-body\">";
      html += "        </div>";
      html += "      </div>";
      html += "    </div>";
      html += "  </div>";
      return html;
    };

    /*
    Provide the HTML for Modal Throbber.
     */
    Drupal.theme.prototype.CToolsModalThrobber = function() {
      var html;
      html = "";
      html += "  <div class=\"loading-spinner\" style=\"position: absolute; top: 45%; left: 50%\">";
      html += "    <i class=\"fa fa-cog fa-spin fa-3x\"></i>";
      html += "  </div>";
      return html;
    };
  })(jQuery);

}).call(this);

//# sourceMappingURL=kalathemeModal.js.map


/**
* @file
* Overrides for Progressbar function
* See misc/batch.js
* Thanks to @link https://drupal.org/project/bootstrap
 */

(function() {
  (function($) {
    var ProgressBar;
    if (window.Drupal == null) {
      window.Drupal = {};
    }

    /*
    A progressbar object. Initialized with the given id. Must be inserted into
    the DOM afterwards through progressBar.element.
    
    method is the function which will perform the HTTP request to get the
    progress bar state. Either "GET" or "POST".
    
    e.g. pb = new progressBar('myProgressBar');
    some_element.appendChild(pb.element);
     */
    ProgressBar = (function() {
      var pb;

      pb = ProgressBar;

      function ProgressBar(id, updateCallback, method, errorCallback) {
        var el, modalHtml;
        this.id = id;
        this.updateCallback = updateCallback;
        this.method = method;
        this.errorCallback = errorCallback;
        el = $("<div class=\"progress-wrapper\" aria-live=\"polite\"></div>");
        modalHtml = "<div id =\"" + this.id + "\" class=\"progress progress-striped active\"";
        modalHtml += "aria-describedby=\"message" + this.id + "\">\n<div class=\"progress-bar\"";
        modalHtml += " role=\"progressbar\" aria-valuemin=\"0\" aria-valuemax=\"100\" ";
        modalHtml += "aria-valuenow=\"0\">\n<div class=\"percentage\">\n</div>\n</div>\n</div>";
        modalHtml += "</div>\n";
        modalHtml += "<p class=\"message\ help-block\" id=\"message" + this.id + "\"></p>";
        el.html(modalHtml);
        this.element = el;
      }


      /*
      Set the percentage and status message for the progressbar.
       */

      ProgressBar.prototype.setProgress = function(percentage, message) {
        if (percentage >= 0 && percentage <= 100) {
          $(".progress-bar", this.element).css("width", percentage + "%");
          $(".progress-bar", this.element).attr("aria-valuenow", percentage);
          $(".percentage", this.element).html(percentage + "%");
        }
        $(".message", this.element).html(message);
        if (this.updateCallback) {
          return this.updateCallback(percentage, message, this);
        }
      };


      /*
      Start monitoring progress via Ajax.
       */

      ProgressBar.prototype.startMonitoring = function(uri, delay) {
        this.delay = delay;
        this.uri = uri;
        return this.sendPing();
      };


      /*
      Stop monitoring progress via Ajax.
       */

      ProgressBar.prototype.stopMonitoring = function() {
        clearTimeout(this.timer);
        return this.uri = null;
      };


      /*
      Request progress data from server.
       */

      ProgressBar.prototype.sendPing = function() {
        if (this.timer) {
          clearTimeout(this.timer);
        }
        if (this.uri) {
          pb = this;
          return $.ajax({
            type: this.method,
            url: this.uri,
            data: "",
            dataType: "json",
            success: function(progress) {
              if (progress.status === 0) {
                pb.displayError(progress.data);
              }
              pb.setProgress(progress.percentage, progress.message);
              return pb.timer = setTimeout(function() {
                return pb.sendPing();
              }, pb.delay);
            },
            error: function(xmlhttp) {
              return pb.displayError(Drupal.ajaxError(xmlhttp, pb.uri));
            }
          });
        }
      };


      /*
      Display errors on the page.
       */

      ProgressBar.prototype.displayError = function(string) {
        var error, errorHtml;
        errorHtml = "<div class=\"alert alert-block alert-error\" role=\"alert\">";
        errorHtml += "<button type=\"button\" class=\"close\"";
        errorHtml += " data-dismiss=\"alert\">&times;</button>";
        errorHtml += "<h4>Error message</h4></div>";
        error = $(errorHtml).append(string);
        $(this.element).before(error).hide();
        if (this.errorCallback) {
          return this.errorCallback(this);
        }
      };

      return ProgressBar;

    })();
    return Drupal.progressBar = ProgressBar;
  })(jQuery);

}).call(this);

//# sourceMappingURL=kalathemeProgress.js.map
