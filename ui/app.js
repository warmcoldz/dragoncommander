const grpc = require('@grpc/grpc-js');
const protoLoader = require('@grpc/proto-loader');
const $ = require('jquery');
const path = require('path');
const fs = require('fs');

const PROTO_PATH = '../proto/file.proto';
const packageDefinition = protoLoader.loadSync(
    PROTO_PATH,
    {keepCase: true,
     longs: String,
     enums: String,
     defaults: true,
     oneofs: true
    });

const file_api = grpc.loadPackageDefinition(packageDefinition).file.api;
const fileExecutor = new file_api.FileExecutor('localhost:50051', grpc.credentials.createInsecure());

const configFile = 'config.json';

let CurrentFolder;
let configValues;

function ReadConfig() {
    try {
        configValues = JSON.parse(fs.readFileSync(configFile));
        return true;
    } catch (e) {
        return false;
    }
}

function Bootstrap() {
    if (!ReadConfig()) {
        configValues = {
            lastDirectory: process.cwd()
        }
    }

    CurrentFolder = configValues.lastDirectory;
    Travel(CurrentFolder);
}

function SaveLastDirectory() {
    configValues.lastDirectory = CurrentFolder;
    fs.writeFileSync(configFile, JSON.stringify(configValues, null, 2));
}

function Travel(currentFolder) {
    CurrentFolder = currentFolder;
    fileExecutor.GetFiles({filePath: currentFolder}, function(err, response) {
        const fileInfo = response.fileInfo;
        fileInfo.forEach((a) => { a.type = (a.type === 'Dir') ? 0 : 1} );
        fileInfo.sort((a, b) => a.type - b.type || a.name.toLowerCase().localeCompare(b.name.toLowerCase()) );

        $('#file-list').empty();
        for (const file of fileInfo) {
            const fullPath = path.resolve(CurrentFolder, file.name);
            const isDir = file.type === 0;
            $('<li/>')
                .attr("class", isDir ? "py-0 list-group-item list-group-item-action list-group-item-info" : "py-0 list-group-item list-group-item-action list-group-item-light")
                .append($('<a/>').attr('href', '#').text(file.name))
                .click(() => { if (isDir) Travel(fullPath); else Execute(fullPath);})
                .appendTo('#file-list')
        }
    });

    $('#path').val(CurrentFolder);
    SaveLastDirectory();
}

function Execute(filePath) {
    fileExecutor.execute({filePath}, function(err, response) {
        console.log(response);
    });
}

function Delete(filePath) {
    fileExecutor.delete({filePath}, function(err, response) {
        console.log(response);
        Travel(CurrentFolder);
    });
}

function CreateDir(filePath) {
    fileExecutor.CreateDir({filePath}, function(err, response) {
        console.log(response);
        Travel(CurrentFolder);
    });
}

$('#navigate-submit').click(function(evt) {
	Travel($('#path').val().trim());
});

$('#path-submit').click(function(evt) {
	Execute($('#path').val().trim());
});

$('#delete-submit').click(function(evt) {
	Delete($('#path').val().trim());
});

$('#createDir-submit').click(function(evt) {
	CreateDir($('#path').val().trim());
});

Bootstrap();
