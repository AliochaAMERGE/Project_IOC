function renderChart(series) {
	JSC.Chart('chartDiv', {
		title_label_text: 'TODO : title',
		annotations: [{
			label_text: 'TODO : annotation',
			position: 'bottom left'
		}],
		legend_visible: false,
		xAxis_crosshair_enabled: true,
		defaultSeries_lastPoint_label_text: '<b>%seriesName</b>',
		defaultPoint_tooltip: '%seriesName <b>%yValue</b> years',
		series: series
	});
}

var getJSON = function(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'json';
    xhr.onload = function() {
      var status = xhr.status;
      if (status === 200) {
        callback(null, xhr.response);
      } else {
        callback(status, xhr.response);
      }
    };
    xhr.send();
};