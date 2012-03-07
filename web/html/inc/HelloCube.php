<?php
require_once 'functions.php';

class HelloCube {
	private $config = null;
	
	function __construct($config) {
		$this->config = $config;
	}
	
	public function execute($req) {
		if(isset($req['act'])) {
			if($req['act'] == 'upload') {
				$this->handleUpload($req);
				exit;			
			}
			else if($req['act'] == 'read_image') {
				$this->readImage($req['f']);
				exit;
			}
			else if($req['act'] == 'image') {
				
				//Twitterbot/1.0
				$agent = strtolower($_SERVER['HTTP_USER_AGENT']);
				if(strstr($agent,'twitter')) {
					$this->readImage($req['f']);
					exit;
				}
				
			}
		}
		else {
			die('unhandled');
			//header('location:http://www.facebook.com/dewars');
			exit;
		}
	}
	
	
	private function handleUpload($req) {

		if(!isset($_FILES['photo'])) {
			l('no photo found in upload');
			$result['result'] = false;
			$result['msg'] = 'no photo was uploaded; or found in post';
			echo json_encode($result);
			exit;

		}
		// Did we get a upload error.
		if($_FILES['photo']['error'] != UPLOAD_ERR_OK) {
			$result['result'] = false;
			$result['msg'] = 'got error code from $_FILES: ' .$_FILES['photo']['error'];
			echo json_encode($result);
			exit;
		}
			
		// check if we can upload the file.
		if(!is_writable($this->config['upload_dir'])) {
			$result['result'] = false;
			$result['msg'] = 'upload dir is not writable';
			echo json_encode($result);
			exit;
		}
			
		// create random filename
		$random = null;
		for ($i = 0; $i < 12; $i++) {
			$random .= chr(rand(97, 122));
		}
		$dir = date('m.d', time());
		
		// upload
		$up = $_FILES['photo'];
		$dest = $this->config['upload_dir'] .'/' .$dir .'/';
		if(!is_dir($dest)) {
			mkdir($dest,0777);
			chmod($dest, 0777);
		}
		$filename = date('H_i', time()) .'_' .$random .'.png';
		$dest .= $filename;
		
		$moved = move_uploaded_file($up['tmp_name'], $dest);
		chmod($dest, 0777);
			
		if(!$moved) {
			$result['result'] = false;
			$result['msg'] = 'error while moving file';
			echo json_encode($result);
			exit;
		}
				
		// success
		$result['result'] = true;
		$result['msg'] = 'success';
		$result['created_file'] = $dir .'/' .$filename;
		//$result['file_hash'] = md5('hellocUbe' .$this->db->lastID());
		echo json_encode($result);
		exit;
	}
	
	
	public function readImage($path) {
		$file = $this->config['upload_dir'] .'/' .$path;
		if(!file_exists($file)) {
			return false;
		}
		
		header('Content-type: image/png');
		header('Content-transfer-encoding: binary');
		header('Content-length: '.filesize($file));
		header('Cache-control: public, max-age=30');
		readfile($file);
		exit;	
	}
		
}

