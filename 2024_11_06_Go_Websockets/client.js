import readline from 'node:readline';

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    prompt: 'Enter message:\n' // Optional prompt to display
});

// Create WebSocket connection.
let socket = null;
try {
    socket = new WebSocket("ws://localhost:8080")
} catch (err) {
    console.error(err)
}

socket.addEventListener("open", (event) => {
    console.log("connection established")
});

socket.addEventListener("message", (event) => {
    console.log("Server:", event.data);
});

rl.on('line', (line) => {
    socket.send(line)
});

