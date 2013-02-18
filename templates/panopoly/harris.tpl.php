<?php
/**
 * @file
 * Template for Panopoly Harris.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display harris clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>
  
  <section class='section alt' id='promo'>
    <div class='container'>
      <div class="harris-container harris-header clearfix panel-panel">
        <div class="harris-container-inner harris-header-inner panel-panel-inner">
          <?php print $content['header']; ?>
        </div>
      </div>
    </div>
  </section>

  <section class='section'>
    <div class='container'>     
      <div class="harris-container harris-column-content clearfix row-fluid">
        <div class="harris-column-content-region harris-column1 panel-panel span3">
          <div class="harris-column-content-region-inner harris-column1-inner panel-panel-inner">
            <?php print $content['column1']; ?>
          </div>
        </div>
        <div class="harris-column-content-region harris-content panel-panel span6">
          <div class="harris-column-content-region-inner harris-content-inner panel-panel-inner">
            <?php print $content['contentmain']; ?>
          </div>
        </div>
        <div class="harris-column-content-region harris-column2 panel-panel span3">
          <div class="harris-column-content-region-inner harris-column2-inner panel-panel-inner">
            <?php print $content['column2']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
    
</div><!-- /.harris -->
