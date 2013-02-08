<?php
/**
 * @file
 * Template for Panopoly Brenham Flipped.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display brenham-flipped clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  
  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="brenham-flipped-container brenham-flipped-header clearfix panel-panel">
        <div class="brenham-flipped-container-inner brenham-flipped-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>
  
  <section class='section'>
    <div class='container'>
      <div class="brenham-flipped-container brenham-flipped-column-content clearfix row-fluid">
        <div class="brenham-flipped-column-content-region brenham-flipped-content panel-panel span8">
          <div class="brenham-flipped-column-content-region-inner brenham-flipped-content-inner panel-panel-inner">
            <?php print $content['contentmain']; ?>
          </div>
        </div>
        <div class="brenham-flipped-column-content-region brenham-flipped-sidebar panel-panel span4">
          <div class="brenham-flipped-column-content-region-inner brenham-flipped-sidebar-inner panel-panel-inner">
            <?php print $content['sidebar']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
  
</div><!-- /.brenham-flipped -->
