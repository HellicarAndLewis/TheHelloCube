<?php
function p($a) {
	if (is_object($a) && get_class($a) !== 'stdClass') {
		$a = array_merge(get_class_vars(get_class($a)), get_class_methods(get_class($a)));
		sort($a);
	}
	echo '<pre><div style="white-space:pre-wrap;margin-top:5px;background-color:#EDF7FF; padding:5px;border:1px solid #D3E0EB;font-family:menlo,monospace;font-size:11px;color:black;" class="debug">'; print_r($a); echo "</div></pre>"; }
	function sl($s) { return addslashes($s); 
}
function e($s) {
	echo $s .'<br>';
}

function l($s) {
	$msg = date('Y:m:d H:i' ."\t", time()) .$s ."\n";
	file_put_contents('log.txt', $msg, FILE_APPEND);
}

if (get_magic_quotes_gpc()) {
	function stripslashes_gpc(&$value)
	{
	    $value = stripslashes($value);
	}
	array_walk_recursive($_GET, 'stripslashes_gpc');
	array_walk_recursive($_POST, 'stripslashes_gpc');
	array_walk_recursive($_COOKIE, 'stripslashes_gpc');
	array_walk_recursive($_REQUEST, 'stripslashes_gpc');
}

function parse_signed_request($signed_request, $secret) {
  list($encoded_sig, $payload) = explode('.', $signed_request, 2); 

  // decode the data
  $sig = base64_url_decode($encoded_sig);
  $data = json_decode(base64_url_decode($payload), true);

  if (strtoupper($data['algorithm']) !== 'HMAC-SHA256') {
    error_log('Unknown algorithm. Expected HMAC-SHA256');
    return null;
  }

  // check sig
  $expected_sig = hash_hmac('sha256', $payload, $secret, $raw = true);
  if ($sig !== $expected_sig) {
    error_log('Bad Signed JSON signature!');
    return null;
  }

  return $data;
}

function base64_url_decode($input) {
  return base64_decode(strtr($input, '-_', '+/'));
}

function truncate($s, $maxChars, $add = '...') {
	$len = strlen($s);
	if($len > $maxChars) {
		$s = substr($s,0,$maxChars) .$add;
	}
	return $s;
}



function cropImage($imageHandle, $x, $y, $w, $h, $sourceWidth, $sourceHeight) {
	$new_handle = imagecreatetruecolor($w, $h);
	$result = imagecopyresampled(
		 $new_handle
		,$imageHandle
		,0
		,0
		,$x
		,$y
		,$w
		,$h
		,$w
		,$h
	);
	if(!$result) {
		return false;
	}
	return $new_handle;
}

function getImageHandle($imageFile, $imageInfo) {
	switch($imageInfo[2]) {
		case IMAGETYPE_JPEG: {
			return imagecreatefromjpeg($imageFile);
			break;
		}
		case IMAGETYPE_PNG: {
			return imagecreatefrompng($imageFile);
			break;
		}
		case IMAGETYPE_GIF: {
			return imagecreatefromgif($imageFile);
			break;
		}
		case IMAGETYPE_BMP: {
			return imagecreatefromwbmp($imageFile);
			break;
		}
		default: {
			return false;
			break;
		}
	}
}

function resizeImage($imageFile, $w, $h, $imageInfo) {
	$src = getImageHandle($imageFile, $imageInfo);
	$thumb = imagecreatetruecolor($w, $h);
	imagecopyresized($thumb, $src, 0,0,0,0, $w, $h, $imageInfo[0], $imageInfo[1]); 
	return $thumb;
}	
