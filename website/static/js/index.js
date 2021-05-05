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
