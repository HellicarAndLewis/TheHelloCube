<?php
ini_set('error_reporting',E_ALL ^ E_NOTICE);


$path = pathinfo(__FILE__);
$base = $path['dirname'] .'/';



// validate if all dirs are writable
// -----------------------------------------------------------------------------
echo_header();

	// check writables and files we need
	check_writable($base .'log.txt');
	check_writable($baes .'uploads');
	check_writable($baes .'uploads/thumb');
	check_writable($baes .'uploads/tiny');
	
echo_footer();


// helpers......
// -----------------------------------------------------------------------------
function check_writable($dir) {
	if(!is_writable($dir)) { 
		err($dir .' is not writable');
	}	
	else {
		succ($dir .' is writable');
	}
}

function check_existance($file) {
	if(file_exists($file)) {
		succ($file .' exists');
		return true;
	}
	else {
		err($file .' not found');
		return false;
	}
}

function err($msg) {
	log_error('<span style="font-size:18px;">✘ </span>' .$msg);
}
function succ($msg) {
	log_success('<span style="font-size:18px;">✔ </span>' .$msg);
}


function echo_header() {
	$s_html = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">'
			.'<html>
				<head>
				<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
				<title>Validate </title>
				</head>
				<body>';
	echo $s_html;
}

function echo_footer() {
	echo '</body>';
}

function e($s, $bg_col = '#EDF7FF', $border_col = '#D3E0EB') { 
	if (is_object($s)) {
		$s = get_class($s);
	}
	if (array_key_exists('SHELL', $_SERVER)) {
		if($s_xhr == 'xmlhttprequest') {
			echo $s;
		}
		else {
			echo $s ."\n";
		}
		return;
	}
	echo '<div style="white-space:wrap;margin-top:5px;background-color:' .$bg_col .'; padding:5px;border:1px solid '.$border_col .';font-family:menlo,monospace;font-size:11px;color:black" class="debug">' ."\n\n\n" .$s  ."\n\n\n".'</div>' ; echo "";
	return $s;
}
function log_error($s) {
	e($s, '#FF2B38', 'darkred');
}
function log_success($s) {
	e($s, '#23FF83', 'darkgreen');
}

