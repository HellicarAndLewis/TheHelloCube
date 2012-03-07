<?php
require_once 'prepend.php';
$hello->execute($_REQUEST);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<link href="/assets/css/image.css" rel="stylesheet" type="text/css" media="screen" />
		<script type="text/javascript" src="/assets/js/mootools/mootools-core-1.4.3.js"></script>
		<script type="text/javascript" src="/assets/js/mootools/mootools-more-1.4.0.1.js"></script>
		<script type="text/javascript" src="/assets/js/image.js"></script>
	
		<title>HelloCube</title>
	</head>
	<body>
	</body>
	
	<div id="site">
		<div id="image">
		<?php
			if($_REQUEST['act'] == 'image') {
				echo '<img src="/?act=read_image&f=' .$_REQUEST['f'] .'" alt="HelloCube" title="HelloCube">';
			}
		?>
		</div>
	</div>
	<div id="footer"></div>
</body>
	
