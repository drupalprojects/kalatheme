<?php
/**
 * @file
 * Template for Panopoly Moscone.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display moscone clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>

  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="moscone-container moscone-header clearfix panel-panel herp-unit">
        <div class="moscone-container-inner moscone-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
      
  <section class='section'>
    <div class='container'>      
      <div class="moscone-container moscone-column-content clearfix row-fluid">
        <div class="moscone-column-content-region moscone-sidebar panel-panel span3">
          <div class="moscone-column-content-region-inner moscone-sidebar-inner panel-panel-inner">
            <?php print $content['sidebar']; ?>
          </div>
        </div>
        <div class="moscone-column-content-region moscone-content panel-panel span9">
          <div class="moscone-column-content-region-inner moscone-content-inner panel-panel-inner">
            <?php print $content['contentmain']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
      
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>
      <div class="moscone-container moscone-footer clearfix panel-panel">
        <div class="moscone-container-inner moscone-footer-inner panel-panel-inner">
          <?php print $content['footer']; ?>
        </div>
      </div>
    </div>
  </footer>
  
</div><!-- /.moscone -->
