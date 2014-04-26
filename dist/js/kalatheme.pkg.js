/*! kalatheme - v3.0.0+dev - 2014-04-25
* https://drupal.org/project/kalatheme
* Copyright (c) 2014 ; Licensed  */
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
