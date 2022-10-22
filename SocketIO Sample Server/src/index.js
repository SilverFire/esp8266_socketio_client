const config = {
    port: 41221,
}

const cors = require('cors');
const app = require('express')();
const http = require('http').createServer(app);
const io = require('socket.io')(http,  {cors: {
  origin: "*",
}});

io.on('connection', (socket) => {
  console.log('Button connected');

  let requestTime;

  const lightButton = () => {
    requestTime = new Date;
    const led = Math.random() > 0.5 ? 'yellow' : 'green'
    socket.emit('pick', {led});
    console.log('Turn on the ' + led + ' LED');
  };

  lightButton();

  socket.on('confirmed', (data) => {
    setTimeout(lightButton, Math.random() * 10000);
    const timeDiffInSeconds = (new Date - requestTime) / 1000;
    console.log('Picked in ' + timeDiffInSeconds + 's');
  })
  
  socket.on('disconnect', () => {
    console.log('Button disconnected');
  })

})

http.listen(config.port, () => {
  console.log("Server launched on port " + config.port);
})
