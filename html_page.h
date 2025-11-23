#ifndef HTML_PAGE_H
#define HTML_PAGE_H

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32-S3 Object Detection</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background: #f0f0f0;
            color: #333;
            margin: 20px;
        }
        h1 {
            margin-bottom: 20px;
        }
        #container {
            position: relative;
            display: inline-block;
            margin: 20px auto;
        }
        #stream {
            display: block;
            max-width: 100%;
            border: 2px solid #333;
        }
        #canvas {
            position: absolute;
            top: 0;
            left: 0;
        }
        #results {
            margin-top: 20px;
            padding: 20px;
            background: white;
            border: 1px solid #ccc;
            display: inline-block;
            min-width: 300px;
        }
        #status {
            margin-top: 10px;
            padding: 10px;
            background: #f9f9f9;
            border: 1px solid #ddd;
        }
        .objects-list {
            margin-top: 20px;
            text-align: left;
        }
        .object-item {
            padding: 8px;
            margin: 5px 0;
            background: #f9f9f9;
            border: 1px solid #ddd;
        }
    </style>
</head>
<body>
    <h1>ESP32-S3 Object Detection</h1>
    
    <div id="container">
        <img id="stream" src="/stream">
        <canvas id="canvas"></canvas>
    </div>
    
    <div id="results">
        <div>Objects Detected: <span id="count">0</span></div>
        <div id="status">Initializing...</div>
        
        <div class="objects-list">
            <h3>Detected Objects:</h3>
            <div id="objects-list"></div>
        </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/@tensorflow/tfjs@4.11.0"></script>
    <script src="https://cdn.jsdelivr.net/npm/@tensorflow-models/coco-ssd@2.2.3"></script>
    
    <script>
        let model = null;
        let isProcessing = false;
        const streamImg = document.getElementById('stream');
        const canvas = document.getElementById('canvas');
        const countSpan = document.getElementById('count');
        const statusDiv = document.getElementById('status');
        const objectsListDiv = document.getElementById('objects-list');

        async function loadModel() {
            try {
                console.log('Loading model...');
                statusDiv.textContent = 'Loading model...';
                model = await cocoSsd.load();
                console.log('Model loaded successfully');
                statusDiv.textContent = 'Model loaded. Detecting every 5 seconds...';
                startDetection();
            } catch (error) {
                console.error('Error:', error);
                statusDiv.textContent = 'Error: ' + error.message;
            }
        }

        function updateCanvas() {
            if (streamImg.offsetWidth > 0) {
                canvas.width = streamImg.offsetWidth;
                canvas.height = streamImg.offsetHeight;
            }
        }

        async function detectObjects() {
            if (!model || isProcessing) return;
            
            isProcessing = true;
            statusDiv.textContent = 'Analyzing...';
            console.log('Running detection...');
            
            try {
                const predictions = await model.detect(streamImg);
                console.log('Predictions:', predictions);
                
                // Clear canvas
                const ctx = canvas.getContext('2d');
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                
                if (predictions && predictions.length > 0) {
                    countSpan.textContent = predictions.length;
                    
                    // Calculate scale factors
                    const scaleX = canvas.width / streamImg.naturalWidth;
                    const scaleY = canvas.height / streamImg.naturalHeight;
                    
                    // Draw bounding boxes
                    predictions.forEach((prediction, idx) => {
                        const [x, y, width, height] = prediction.bbox;
                        
                        // Scale coordinates
                        const scaledX = x * scaleX;
                        const scaledY = y * scaleY;
                        const scaledWidth = width * scaleX;
                        const scaledHeight = height * scaleY;
                        
                        // Draw box
                        ctx.strokeStyle = '#00ff00';
                        ctx.lineWidth = 3;
                        ctx.strokeRect(scaledX, scaledY, scaledWidth, scaledHeight);
                        
                        // Draw label
                        const label = prediction.class + ' ' + (prediction.score * 100).toFixed(0) + '%';
                        ctx.fillStyle = '#00ff00';
                        ctx.font = '16px Arial';
                        ctx.fillText(label, scaledX, scaledY > 20 ? scaledY - 5 : scaledY + 20);
                    });
                    
                    // Update list
                    let listHTML = '';
                    predictions.forEach((pred, idx) => {
                        const confidence = (pred.score * 100).toFixed(1);
                        listHTML += `<div class="object-item">${idx + 1}. ${pred.class} (${confidence}%)</div>`;
                    });
                    objectsListDiv.innerHTML = listHTML;
                    
                    statusDiv.textContent = 'Updated: ' + new Date().toLocaleTimeString();
                } else {
                    countSpan.textContent = '0';
                    objectsListDiv.innerHTML = '<div>No objects detected</div>';
                    statusDiv.textContent = 'No objects in frame';
                }
            } catch (error) {
                console.error('Detection error:', error);
                statusDiv.textContent = 'Error: ' + error.message;
            }
            
            isProcessing = false;
        }

        function startDetection() {
            detectObjects();
            setInterval(detectObjects, 2000);
        }

        streamImg.onload = updateCanvas;
        window.addEventListener('resize', updateCanvas);
        setInterval(updateCanvas, 1000);

        window.addEventListener('load', () => {
            console.log('Page loaded, starting model load...');
            loadModel();
        });
    </script>
</body>
</html>
)rawliteral";

#endif