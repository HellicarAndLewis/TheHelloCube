<?php
require_once 'serverswitch.php';
$server_switch = new ServerSwitch();

$config['upload_dir'] = $base .'/uploads';
$server_switch->addServer('production', 'production', false, $config);
$config = $server_switch->getConfig('production');

