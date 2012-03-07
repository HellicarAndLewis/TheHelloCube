<?php
date_default_timezone_set('Europe/London');
$path = pathinfo(__FILE__);
$base = $path['dirname'];

require_once $base .'/config/config.php';
require_once $base .'/inc/functions.php';
require_once $base .'/inc/HelloCube.php';

$hello = new HelloCube($config);


