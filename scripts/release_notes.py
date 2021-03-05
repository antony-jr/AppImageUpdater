import sys

if len(sys.argv) < 3:
    print("Usage: release_notes.py [GIT COMMIT MESSAGE] [OUTPUT FILE]")
    sys.exit(-1)

message = sys.argv[1];
output_file = sys.argv[2];

if "RELEASE" in message:
    if message.find('\n\n') != -1:
        release_notes = message[message.find('\n\n') + 2:]
        with open(output_file, "w") as fp:
            fp.write(release_notes)
        sys.exit(0)

with open(output_file, "w") as fp:
    fp.write("")
sys.exit(0)
