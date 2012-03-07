<?php
class ServerSwitch {
	private $addresses = array();
	
	public function addServer($key, $name, $isProduction, $extraConfig) {
		$this->addresses[$key] = array(
			'ip' => $key
			,'name' => $name
			,'production' => $isProduction
			,'config' => $extraConfig
		);
	}
	
	public function getConfig($name = null) {
		if($name == null) {
			$name = $this->getIP();
		}
		return $this->addresses[$name]['config'];
	}
	
	public function isProduction() {
		if(is_array($this->addresses[$this->getIP()])) {
			return $this->addresses[$this->getIP()]['production'];
		}
	}
	
	private function getIP() {
		return $_SERVER['SERVER_ADDR'];
	}
	
}
$switch = new ServerSwitch();
