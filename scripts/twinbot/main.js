const teeworlds = require('teeworlds');
const fs = require('fs');
const readline = require('readline');

const servers = [];

const fileStream = fs.createReadStream('ip.txt');
const rl = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity
});

rl.on('line', (line) => {
    if (line.trim()) {
        const [ip, port] = line.trim().split(':');
        servers.push({ ip, port: parseInt(port) });
    }
});

rl.on('close', () => {
    servers.forEach((server, index) => {
        setTimeout(() => {
            createBot(server.ip, server.port, 'TUX');
        }, index * 100);
    });
});

function createBot(ip, port, botName) {
    let client = new teeworlds.Client(ip, port, botName);
    let messageToAD = 0;
    
    client.on("connected", () => {
        console.log(`Connected to ${ip}:${port}`);
    });

    client.on("disconnect", reason => {
        console.log(`Disconnect from ${ip}:${port} ${reason}`);
    });

    client.on("message", message => {
        if (message.client_id == -1 || message.author.ClientInfo == undefined) {
            return;
        }

        if (messageToAD == 1) {
            client.game.ChangePlayerInfo({
                name: `${message.author.ClientInfo.name}.`,
                clan: message.author.ClientInfo.clan,
                country: message.author.ClientInfo.country,
                skin: message.author.ClientInfo.skin,
                use_custom_color: message.author.ClientInfo.use_custom_color,
                color_body: message.author.ClientInfo.color_body,
                color_feet: message.author.ClientInfo.color_feet,
            });
        } else if (messageToAD == 5) {
            client.game.ChangePlayerInfo({
                name: `${message.author.ClientInfo.name}.`,
                clan: message.author.ClientInfo.clan,
                country: message.author.ClientInfo.country,
                skin: message.author.ClientInfo.skin,
                use_custom_color: message.author.ClientInfo.use_custom_color,
                color_body: message.author.ClientInfo.color_body,
                color_feet: message.author.ClientInfo.color_feet,
            });
        } else if (messageToAD == 10) {
            client.game.Say("Зоходи на сервер twinboobs ddrace.ru ммо рпг мод что-бы ввойти введи connect ddrace.ru");
            messageToAD = 0;
        }

        messageToAD = messageToAD + 1;
    });

    client.connect();
}

process.on('SIGINT', () => {
    console.log('\nЗавершение работы...');
    process.exit();
});
