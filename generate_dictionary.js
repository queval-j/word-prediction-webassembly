const WORD_LIMIT = 60000;
const fs = require('fs');

function formatWord(word, weight) {
    return `${word.length}"${word.toLowerCase()}"${weight}`;
}

const WEIGHT_LIMIT = 2 * 1000 * 1000 * 100; // 200M
const lines = fs.readFileSync('./words.csv').toString().split('\n').splice(0, WORD_LIMIT);
lines.push('Hello World,'+WEIGHT_LIMIT);
lines.push('Lok\'tar,'+WEIGHT_LIMIT);
lines.push('Mommy,'+WEIGHT_LIMIT);
lines.push('Daddy,'+WEIGHT_LIMIT);
lines.push('Hello,'+WEIGHT_LIMIT);
const fileContent = lines.map((line) => {
    let [word, weight] = line.split(',');
    weight = +((+weight / 10).toFixed(0));
    weight = (weight > WEIGHT_LIMIT ? WEIGHT_LIMIT : weight);
    word = word.trim();
    console.log(word, weight);
    return formatWord(word, weight);
}).join("\n");
console.log('size:', fileContent.length);
fs.writeFileSync('./dictionary.mydb', fileContent);

// fs.appendFile('./dictionary.mydb', data, () => {
//     console.log('Word added!');
// });
