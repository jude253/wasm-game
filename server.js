
// Express
const express = require('express')
const app = express()

// Serve static files from /public
app.use( express.static('public', {
    setHeaders: (res, path, stat) => {
        if (path.endsWith('.wasm')) {
            res.set('Content-Type', 'application/wasm')
        }
    }
}) )

app.listen( 2222, () => console.log('LINK:\n\thttp://localhost:2222\n\nServer running on Port 2222!'))