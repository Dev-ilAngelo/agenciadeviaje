/**
 * ARCHITECT: Frontend Senior
 * PROJECT: Travel Management System (Legacy C to Vanilla JS)
 * LOGIC: Deterministic Business Rules mapped from TPViajeMagicodefinitivo.cpp
 */

const CONFIG = {
    MAX_PASSENGERS: 240,
    CUPO_MAX: 60,
    MENOR_AGE_LIMIT: 5,
    MENOR_SURCHARGE: 2000,
    CARD_SURCHARGE_PCT: 0.05,
    DESTINATIONS: {
        'BRA': { name: 'Brasil', price: 25000 },
        'MDQ': { name: 'Mar del Plata', price: 14000 },
        'MZA': { name: 'Mendoza', price: 19000 },
        'BRC': { name: 'Bariloche', price: 23000 }
    }
};

class TravelSystem {
    constructor() {
        this.passengers = JSON.parse(localStorage.getItem('passengers')) || [];
        this.init();
    }

    init() {
        this.renderStats();
        this.renderList();
    }

    // --- LOGIC MAPPED FROM C ---

    validateDNI(dni) {
        if (!/^\d{7,8}$/.test(dni)) return "El DNI debe tener 7 u 8 números.";
        const num = parseInt(dni);
        if (num < 5000000 || num > 60000001) return "DNI fuera de rango (5M - 60M).";
        if (this.passengers.some(p => p.dni === dni)) return "Este DNI ya fue cargado.";
        return null;
    }

    checkCupo(destCode) {
        const count = this.passengers.filter(p => p.destinationCode === destCode).length;
        return count < CONFIG.CUPO_MAX;
    }

    calculateImporte(age, destCode, isCard) {
        let importe = CONFIG.DESTINATIONS[destCode].price;
        if (age <= CONFIG.MENOR_AGE_LIMIT) importe += CONFIG.MENOR_SURCHARGE;
        if (isCard) importe += (importe * CONFIG.CARD_SURCHARGE_PCT);
        return importe;
    }

    addPassenger(data) {
        const error = this.validateDNI(data.dni);
        if (error) throw new Error(error);

        if (!this.checkCupo(data.destinationCode)) {
            throw new Error("Sin cupo para este destino.");
        }

        const importe = this.calculateImporte(data.age, data.destinationCode, data.isCard);
        
        const newPassenger = {
            id: Date.now(),
            dni: data.dni,
            nombre: data.nombre.trim(),
            apellido: data.apellido.trim(),
            age: parseInt(data.age),
            destinationCode: data.destinationCode,
            destinationName: CONFIG.DESTINATIONS[data.destinationCode].name,
            importe: importe,
            isCard: data.isCard,
            createdAt: new Date().toISOString()
        };

        this.passengers.push(newPassenger);
        this.save();
        return newPassenger;
    }

    save() {
        localStorage.setItem('passengers', JSON.stringify(this.passengers));
        this.renderStats();
        this.renderList();
    }

    // --- ANALYTICS MAPPED FROM C (estadisticas function) ---

    getStats() {
        const stats = {
            totalRecaudado: this.passengers.reduce((sum, p) => sum + p.importe, 0),
            byDestination: {}
        };

        Object.keys(CONFIG.DESTINATIONS).forEach(code => {
            const passengersInDest = this.passengers.filter(p => p.destinationCode === code);
            const minors = passengersInDest.filter(p => p.age <= 5).length;
            
            stats.byDestination[code] = {
                name: CONFIG.DESTINATIONS[code].name,
                count: passengersInDest.length,
                recaudacion: passengersInDest.reduce((sum, p) => sum + p.importe, 0),
                ocupacionPct: (passengersInDest.length * 100 / CONFIG.CUPO_MAX).toFixed(2),
                minorsPct: this.passengers.length > 0 ? (minors * 100 / this.passengers.length).toFixed(2) : 0
            };
        });

        // Destination with most passengers
        const codes = Object.keys(stats.byDestination);
        const mostRequestedCode = codes.reduce((a, b) => 
            stats.byDestination[a].count >= stats.byDestination[b].count ? a : b
        );
        stats.mostRequested = stats.byDestination[mostRequestedCode].name;

        return stats;
    }

    // --- UI RENDERING ---

    renderStats() {
        const stats = this.getStats();
        const container = document.getElementById('stats-summary');
        if (!container) return;

        container.innerHTML = `
            <div class="stat-card">
                <div class="stat-label">Total Recaudado</div>
                <div class="stat-value">$${stats.totalRecaudado.toLocaleString()}</div>
            </div>
            <div class="stat-card">
                <div class="stat-label">Destino más solicitado</div>
                <div class="stat-value text-accent">${stats.mostRequested}</div>
            </div>
            <div class="stat-card">
                <div class="stat-label">Total Pasajeros</div>
                <div class="stat-value">${this.passengers.length}</div>
            </div>
        `;

        const details = document.getElementById('stats-details');
        if (details) {
            details.innerHTML = Object.values(stats.byDestination).map(d => `
                <div class="stat-card">
                    <div class="stat-label">${d.name}</div>
                    <div class="stat-value" style="font-size: 1.2rem">${d.ocupacionPct}% Ocupación</div>
                    <div class="stat-label" style="font-size: 0.7rem">Menores: ${d.minorsPct}% del total</div>
                </div>
            `).join('');
        }
    }

    renderList(filterDni = '') {
        const tbody = document.getElementById('passenger-tbody');
        if (!tbody) return;

        // Sorting: Lastname then Firstname (as in C: ordenarPasajeros)
        let displayList = [...this.passengers].sort((a, b) => {
            const last = a.apellido.localeCompare(b.apellido);
            if (last !== 0) return last;
            return a.nombre.localeCompare(b.nombre);
        });

        if (filterDni) {
            displayList = displayList.filter(p => p.dni.includes(filterDni));
        }

        tbody.innerHTML = displayList.map(p => `
            <tr>
                <td><strong>${p.apellido.toUpperCase()}, ${p.nombre}</strong></td>
                <td><code style="opacity: 0.7">${p.dni}</code></td>
                <td>${p.age} años</td>
                <td><span class="status-badge badge-success">${p.destinationName}</span></td>
                <td class="text-accent font-bold">$${p.importe.toLocaleString()}</td>
            </tr>
        `).join('');
    }
}

// Initialization and Event Listeners
const app = new TravelSystem();

function showSection(id) {
    document.querySelectorAll('.section').forEach(s => s.classList.add('hidden'));
    document.getElementById(id).classList.remove('hidden');
    document.querySelectorAll('.nav-tab').forEach(t => t.classList.remove('active'));
    document.querySelector(`[onclick="showSection('${id}')"]`).classList.add('active');
}

document.getElementById('passenger-form')?.addEventListener('submit', (e) => {
    e.preventDefault();
    const formData = new FormData(e.target);
    try {
        app.addPassenger({
            dni: formData.get('dni'),
            nombre: formData.get('nombre'),
            apellido: formData.get('apellido'),
            age: formData.get('age'),
            destinationCode: formData.get('destination'),
            isCard: formData.get('payment') === 'CARD'
        });
        alert('✅ Pasajero cargado con éxito');
        e.target.reset();
        showSection('section-list');
    } catch (err) {
        alert('❌ Error: ' + err.message);
    }
});

function handleSearch(val) {
    app.renderList(val);
}
