var s = null;


var Site = new Class({
	initialize: function() {
		var me = this;
		window.addEvent('resize',function() {
			me.resize();
		});
		me.resize();
		me.image = $('image');
	}
	
	,resize: function() {
		var s = window.getSize();
		var w = s.x * 0.8;
		$('image').setStyles({
				'margin-left': -w*0.5 +"px"
				,'left':"50%"
				,"width":w +"px"
		});
		$('image').getElement('img').setStyle('width', w);
	}
});

document.addEvent('domready', function() {
	s = new Site();
});
	
	
