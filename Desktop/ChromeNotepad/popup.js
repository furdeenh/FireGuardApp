document.addEventListener('DOMContentLoaded', function() {
    var notepad = document.getElementById('notepad');
    var noteSelect = document.getElementById('noteSelect');
    var newNoteButton = document.getElementById('newNote');
    var deleteNoteButton = document.getElementById('deleteNote');

    // Load existing notes
    chrome.storage.sync.get('notes', function(data) {
        var notes = data.notes || {'default': ''};
        for (var key in notes) {
            var option = document.createElement('option');
            option.value = key;
            option.textContent = key;
            noteSelect.appendChild(option);
        }
        notepad.value = notes[noteSelect.value];
    });

    // Save the note when its content changes
    notepad.addEventListener('input', function() {
        var notes = {};
        notes[noteSelect.value] = notepad.value;
        chrome.storage.sync.set({'notes': notes});
    });

    // Handle note selection
    noteSelect.addEventListener('change', function() {
        chrome.storage.sync.get('notes', function(data) {
            var notes = data.notes || {};
            notepad.value = notes[noteSelect.value] || '';
        });
    });

    // Create a new note
    newNoteButton.addEventListener('click', function() {
        var noteName = prompt("Enter the name for the new note:");
        if (noteName) {
            var option = document.createElement('option');
            option.value = noteName;
            option.textContent = noteName;
            noteSelect.appendChild(option);
            noteSelect.value = noteName;
            notepad.value = '';
            notepad.focus();
        }
    });


document.addEventListener('DOMContentLoaded', function() {
    var fontSelect = document.getElementById('fontSelect');
    var notepad = document.getElementById('notepad');

    fontSelect.addEventListener('change', function() {
        notepad.style.fontFamily = fontSelect.value;
    });
});


    // Delete the current note
    deleteNoteButton.addEventListener('click', function() {
        var confirmDelete = confirm("Are you sure you want to delete this note?");
        if (confirmDelete) {
            chrome.storage.sync.get('notes', function(data) {
                var notes = data.notes || {};
                delete notes[noteSelect.value];
                chrome.storage.sync.set({'notes': notes}, function() {
                    noteSelect.remove(noteSelect.selectedIndex);
                    notepad.value = '';
                });
            });
        }
    });
});

