<?php
/**
 * @file
 * Template for Panopoly Sanderson.
 *
 * Variables:
 * - $css_id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 * panel of the layout. This layout supports the following sections:
 */
?>

<div class="panel-display sanderson-flipped clearfix <?php !empty($class) ? print $class : ''; ?>" <?php !empty($css_id) ? print "id=\"$css_id\"" : ''; ?>>

  <section class='section'>
    <div class='container'>
      <div class="sanderson-flipped-container sanderson-flipped-column-content sanderson-flipped-column-content-row1 clearfix row-fluid">
        <div class="sanderson-flipped-column-content-region sanderson-flipped-column1 panel-panel span4">
          <div class="sanderson-flipped-column-content-region-inner sanderson-flipped-column1-inner panel-panel-inner">
            <?php print $content['column1']; ?>
          </div>
        </div>
        <div class="sanderson-flipped-column-content-region sanderson-flipped-column2 panel-panel span4">
          <div class="sanderson-flipped-column-content-region-inner sanderson-flipped-column2-inner panel-panel-inner">
            <?php print $content['column2']; ?>
          </div>
        </div>
        <div class="sanderson-flipped-column-content-region sanderson-flipped-column3 panel-panel span4">
          <div class="sanderson-flipped-column-content-region-inner sanderson-flipped-column3-inner panel-panel-inner">
            <?php print $content['column3']; ?>
          </div>
        </div>
      </div>
    </div>
  </section>
  
  <footer class='section' id='footer' role='contentinfo'>
    <div class='container'>  
      <div class="sanderson-flipped-container sanderson-flipped-secondary-column-content sanderson-flipped-column-content-row2 clearfix row-fluid">
        <div class="sanderson-flipped-secondary-column-content-region sanderson-flipped-secondary-column1 panel-panel span6">
          <div class="sanderson-flipped-secondary-column-content-region-inner sanderson-flipped-secondary-column1-inner panel-panel-inner">
            <?php print $content['secondarycolumn1']; ?>
          </div>
        </div>
        <div class="sanderson-flipped-secondary-column-content-region sanderson-flipped-secondary-column2 panel-panel span6">
          <div class="sanderson-flipped-secondary-column-content-region-inner sanderson-flipped-secondary-column2-inner panel-panel-inner">
            <?php print $content['secondarycolumn2']; ?>
          </div>
        </div>
      </div>
    </div>
  </footer>
    
</div><!-- /.sanderson-flipped -->
