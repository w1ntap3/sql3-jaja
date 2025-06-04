let totalRecords = 0;
let tableCount = 0;

function formatFileSize(bytes) {
    if (bytes === 0) return '0 bytes';
    const k = 1024;
    const sizes = ['bytes', 'kb', 'mb', 'gb'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

function animateNumber(element, target, duration = 2000) {
    const start = 0;
    const startTime = performance.now();
    
    function update(currentTime) {
    const elapsed = currentTime - startTime;
    const progress = Math.min(elapsed / duration, 1);
    const current = Math.floor(start + (target - start) * progress);
    element.textContent = current.toLocaleString();
    
    if (progress < 1) {
        requestAnimationFrame(update);
    }
    }
    
    requestAnimationFrame(update);
}

document.getElementById('fileInput').addEventListener('change', async (event) => {
    const file = event.target.files[0];
    if (!file) return;

    const loading = document.getElementById('loading');
    const output = document.getElementById('output');
    const statsBar = document.getElementById('statsBar');
    
    loading.classList.add('active');
    output.innerHTML = '';
    statsBar.style.display = 'none';
    totalRecords = 0;
    tableCount = 0;

    try {
    const arrayBuffer = await file.arrayBuffer();
    const SQL = await initSqlJs({ 
        locateFile: filename => `https://cdnjs.cloudflare.com/ajax/libs/sql.js/1.7.0/${filename}` 
    });
    const db = new SQL.Database(new Uint8Array(arrayBuffer));

    const tables = db.exec("SELECT name FROM sqlite_master WHERE type='table';");
    
    loading.classList.remove('active');

    if (tables[0]) {
        tableCount = tables[0].values.length;
        
        tables[0].values.forEach(([tableName], index) => {
        setTimeout(() => {
            const tableContainer = document.createElement('div');
            tableContainer.className = 'table-container';
            
            const tableHeader = document.createElement('div');
            tableHeader.className = 'table-header';
            tableHeader.textContent = `table: ${tableName.toLowerCase()}`;
            
            const tableData = db.exec(`SELECT * FROM ${tableName};`);
            
            if (tableData[0]) {
            const columns = tableData[0].columns;
            const values = tableData[0].values;
            totalRecords += values.length;
            
            let html = '<table class="data-table"><thead><tr>';
            html += columns.map(col => `<th>${col.toLowerCase()}</th>`).join('');
            html += '</tr></thead><tbody>';
            
            values.forEach(row => {
                html += '<tr>' + row.map(val => `<td>${val !== null ? val : '<em>null</em>'}</td>`).join('') + '</tr>';
            });
            html += '</tbody></table>';
            
            tableContainer.innerHTML = tableHeader.outerHTML + html;
            } else {
            tableContainer.innerHTML = tableHeader.outerHTML + '<div class="no-data">no data found in this table</div>';
            }
            
            output.appendChild(tableContainer);
            
            // Show stats after last table
            if (index === tables[0].values.length - 1) {
            setTimeout(() => {
                statsBar.style.display = 'flex';
                animateNumber(document.getElementById('tableCount'), tableCount);
                animateNumber(document.getElementById('recordCount'), totalRecords);
                document.getElementById('fileSize').textContent = formatFileSize(file.size);
            }, 300);
            }
        }, index * 200);
        });
    } else {
        output.innerHTML = '<div class="error-message">no tables found in this database.</div>';
    }
    } catch (error) {
    loading.classList.remove('active');
    output.innerHTML = `<div class="error-message">error reading database: ${error.message.toLowerCase()}</div>`;
    }
});

// Add click handler for the button to trigger file input
document.querySelector('.file-input-button').addEventListener('click', (e) => {
    e.preventDefault();
    document.getElementById('fileInput').click();
});