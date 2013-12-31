<?php

/**
 * @file
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 *
 * @ingroup views_templates
 */
?>

<?php
reset($rows);
$gridsize = count($rows[0]);
?>

<?php if (!empty($title)) : ?>
<h3 class='grid-title'>
  <?php print $title; ?>
</h3>
<?php endif; ?>

<div class="views-view-grid grid-<?php print $gridsize ?>">
  <?php foreach ($rows as $row_number => $columns): ?>
  <?php
    $row_class = 'row-' . ($row_number + 1);
    $row_class .= ($row_number == 0 && count($rows) > 1) ? ' row-first' : '';
    $row_class .= (count($rows) == ($row_number + 1)) ? ' row-last' : '';
    $row_class .= ' row';
  ?>
  <div class="<?php print $row_class; ?>">
    <?php foreach ($columns as $column_number => $item): ?>
    <div
      class="gridCol gridborder <?php print 'col-' . ($column_number + 1); ?> <?php $span ? print $span : ''; ?>">
      <?php if ($item): ?>
      <div class='grid-item'>
        <?php print $item; ?>
      </div>
      <?php endif; ?>
    </div>
    <?php endforeach; ?>
  </div>
  <?php endforeach; ?>
</div>
