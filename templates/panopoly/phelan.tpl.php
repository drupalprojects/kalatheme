<?php
/**
 * @file
 * Template for Panopoly Phelan.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display phelan clearfix <?php if (!empty($class)) { print $class; } ?>" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <section class='section'>
    <div class='container'>
      <div class="phelan-container phelan-column-content clearfix row-fluid">
        <div class="phelan-column-content-region phelan-column1 phelan-column panel-panel span6">
          <div class="phelan-column-content-region-inner phelan-column1-inner phelan-column-inner panel-panel-inner">
            <?php print $content['column1']; ?>
          </div>
        </div>
        <div class="phelan-column-content-region phelan-column2 phelan-column panel-panel span6">
          <div class="phelan-column-content-region-inner phelan-column2-inner phelan-column-inner panel-panel-inner">
            <?php print $content['column2']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
    
</div><!-- /.phelan -->
