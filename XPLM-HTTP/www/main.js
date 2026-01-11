var map = null;
var player =  L.circle([50.89955816484794, 4.490048490400604], {
        color: 'red',
        fillColor: '#f03',
        fillOpacity: 0.5,
        radius: 90
});

var playerHistory = L.polyline([], {
    color: "#ff7800",
    weight: 5,
    opacity: 0.65
});

var altitudeChart = null;

window.addEventListener('DOMContentLoaded', (event) => {
    initializeMap();
    initCharts();
    window.setInterval(updatePosition, 1000);
});

async function updatePosition(){
    const response = await fetch('API/Position');
    if(response.ok){
        const value = await response.json();
        const position = value.Value;
        player.setLatLng([position.latitude, position.longitude]);
        player.bindPopup(`Alt: ${position.elevation * 3.28084}<br>Speed: ${position.ground_speed * 1.94384}`).openPopup();
        playerHistory.addLatLng([position.latitude, position.longitude]);
        altitudeChart.data.labels.push(new Date().getUTCHours() + ":" + new Date().getUTCMinutes() + ":" + new Date().getUTCSeconds()); 
        altitudeChart.data.datasets[0].data.push(position.elevation * 3.28084);
        altitudeChart.data.datasets[1].data.push(position.true_airspeed * 1.94384);
        altitudeChart.data.datasets[2].data.push(position.equiv_airspeed);
        altitudeChart.update();
    }
}

function initCharts(){
    altitudeChart = new Chart(document.getElementById('altitude'),
    {
        type: 'line',
        data: {
        labels: [],
        datasets: [
            {
                label: 'altitude',
                data: [],
                borderWidth: 1,
                yAxisID: 'y'
            },
            {
                label: 'tas',
                data: [],
                borderWidth: 1,
                yAxisID: 'y1'
            },
            {
                label: 'eas',
                data: [],
                borderWidth: 1,
                yAxisID: 'y1'
            }
        ]
        },
        options: {
        scales: {
            y: {
            beginAtZero: true
            }
        }
        }
    });
}

function initializeMap() {
    map = L.map('map').setView([50.89955816484794, 4.490048490400604], 12);
    L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
        maxZoom: 19,
        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    }).addTo(map);

    player.addTo(map);
    playerHistory.addTo(map);
}