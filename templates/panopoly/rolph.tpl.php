<?php
/**
 * @file
 * Template for Panopoly Rolph.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display rolph clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="rolph-container rolph-header clearfix panel-panel ">
        <div class="rolph-container-inner rolph-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>         
      <div class="rolph-container rolph-quarter-content clearfix row-fluid">
        <div class="rolph-column-content-region rolph-quarter1 rolph-quarter panel-panel span3">
          <div class="rolph-column-content-region-inner rolph-quarter1-inner rolph-quarter-inner panel-panel-inner">
            <?php print $content['quarter1']; ?>
          </div>
        </div>
        <div class="rolph-column-content-region rolph-quarter2 rolph-quarter panel-panel span3">
          <div class="rolph-column-content-region-inner rolph-quarter2-inner rolph-quarter-inner panel-panel-inner">
            <?php print $content['quarter2']; ?>
          </div>
        </div>
        <div class="rolph-column-content-region rolph-quarter3 rolph-quarter panel-panel span3">
          <div class="rolph-column-content-region-inner rolph-quarter3-inner rolph-quarter-inner panel-panel-inner">
            <?php print $content['quarter3']; ?>
          </div>
        </div>
        <div class="rolph-column-content-region rolph-quarter4 rolph-quarter panel-panel span3">
          <div class="rolph-column-content-region-inner rolph-quarter4-inner rolph-quarter-inner panel-panel-inner">
            <?php print $content['quarter4']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
  
   <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>       
      <div class="rolph-container rolph-footer clearfix panel-panel">
        <div class="rolph-container-inner rolph-footer-inner panel-panel-inner">
          <?php print $content['footer']; ?>
        </div>
      </div>
    </div>
  </footer>
    
</div><!-- /.rolph -->
