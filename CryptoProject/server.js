const express = require('express');
const app = express();
const favicon = require('express-favicon');
const path = require('path');
const fs = require('fs')
const { exec } = require("child_process");
const cors = require('cors')
const port = process.env.PORT || 3000;

app.use(cors())

app.use(favicon(__dirname + '/build/favicon.ico'));
app.use(express.static(__dirname));
app.use(express.static(path.join(__dirname, 'build')));
app.use(express.json({ limit: '50mb' }))
app.use(express.urlencoded({ limit: '50mb', extended: true }))

app.get('/*', function (req, res) {
  res.sendFile(path.join(__dirname, 'build', 'index.html'));
});

app.post('/api',(req, res)=>{
    const require_auth = req.body.require_auth?1:0
    const forging = req.body.forging?1:0
    fs.writeFileSync('InputFiles/input_plain_txt_message.txt', req.body.plain_text)
    fs.writeFileSync('InputFiles/chacha20_key.txt', req.body.chacha_key)
    fs.writeFileSync('InputFiles/chacha20_nonce.txt', req.body.chacha_nonce)
    fs.writeFileSync('InputFiles/chacha20_counter.txt', req.body.chacha_counter.toString())

    if(req.body.require_auth){
        fs.writeFileSync('InputFiles/blake2b-512_digest_size.txt', req.body.blake_digest_size.toString())
        fs.writeFileSync('InputFiles/blake2b-512_key.txt', req.body.blake_key)  
    }

    exec(`bash execute.sh ${require_auth} ${forging}`, (error, stdout, stderr) => {
        if (error) {
            return res.status(400).json({'error':'Bad Request'});
        }

        let output = {}
        const decryptedText = fs.readFileSync('OutputFiles/decrypted_text.txt', 'utf8')
        const cipherAlice = fs.readFileSync('OutputFiles/cipher_text_generated_by_Alice.txt', 'utf8')
        const cipherBob = fs.readFileSync('OutputFiles/cipher_text_reaching_Bob.txt', 'utf8')
        
        output['decrypted_text'] = decryptedText
        output['cipher_alice'] = cipherAlice
        output['cipher_bob'] = cipherBob

        if(req.body.require_auth){
            const hashAlice = fs.readFileSync('OutputFiles/hash_alice.txt', 'utf8')
            const hashBob = fs.readFileSync('OutputFiles/hash_bob.txt', 'utf8')
            const tagVerification = fs.readFileSync('OutputFiles/tag_verification_result.txt', 'utf8')
            output['hash_alice'] = hashAlice
            output['hash_bob'] = hashBob
            output['tag_verification'] = tagVerification
            output['auth'] = true 
        }else{
            output['auth'] = false
        }
       
        return res.json(output);
    });
})

app.post('/hmac',(req, res)=>{
    fs.writeFileSync('InputFiles/input_plain_txt_message.txt', req.body.text_message)
    fs.writeFileSync('InputFiles/blake2b-512_key.txt', req.body.blake_key)
    fs.writeFileSync('InputFiles/blake2b-512_digest_size.txt', req.body.digest_bytes.toString())
    exec(`bash execute.sh`, (error, stdout, stderr) => {
        if (error) {
            return res.status(400).json({'error':'Bad Request'});
        }

        let output = {}
        const hashTag = fs.readFileSync('OutputFiles/hmac_demonstration.txt', 'utf8')
        output['hash_tag'] = hashTag
        return res.json(output);
    })
})



app.listen(port,() =>{
    console.log(`Server started on port ${port} ...`)
})

