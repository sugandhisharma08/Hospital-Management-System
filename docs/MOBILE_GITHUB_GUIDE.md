# Uploading to GitHub from an Android Phone

GitHub's website does **not** unzip a ZIP file for you, and the Android file picker cannot select a
whole folder. So you upload the extracted files folder by folder. It takes about 10 minutes.

## Part 1 - Get the files on your phone
1. Download `Hospital-Management-System.zip` from Claude (tap the file card, then Download).
2. Open **Files by Google** (or your phone's file manager) -> Downloads.
3. Tap the ZIP -> **Extract**. You now have a folder `Hospital-Management-System` with `src`, `include`, `data`, `docs`.

## Part 2 - Create the repository
1. Open Chrome -> github.com -> sign in (create a free account if needed).
2. Tap your profile picture -> **Your repositories** -> **New** (or go to github.com/new).
3. Repository name: e.g. `Hospital-Management-System`. Choose **Public** (or Private if your teacher can be added).
4. Tick **Add a README file**? **No** (leave it unticked, our ZIP has its own README).
5. Tap **Create repository**.

## Part 3 - Upload the files (easiest way)
1. In Chrome tap the three dots -> tick **Desktop site** (the upload buttons are easier to see).
2. On your empty repository page tap **uploading an existing file** (or **Add file -> Upload files**).
3. Tap **choose your files**, open the extracted project folder and select the **loose files in the root**:
   `README.md`, `Makefile`, `.gitignore`. (If `.gitignore` is hidden, turn on "Show hidden files" in the file manager.)
4. Scroll down, tap **Commit changes**.

### Create the sub-folders (one-time trick)
GitHub only lets you upload into a folder that already exists. Create each folder like this:
1. **Add file -> Create new file**.
2. In the name box type `src/placeholder.txt` (typing the `/` creates the folder). Type any letter in the file body.
3. Tap **Commit changes**. Repeat for `include/placeholder.txt`, `data/placeholder.txt` and `docs/placeholder.txt`.

### Upload files into each folder
1. Tap the `src` folder in the repository.
2. **Add file -> Upload files -> choose your files** -> open the extracted `src` folder -> tap the menu (three dots) -> **Select all** -> Open.
3. Wait until all file names are listed, then **Commit changes**.
4. Repeat for `include`, `data` and `docs`.
5. Optional: delete each `placeholder.txt` (open it -> three dots -> **Delete file** -> Commit).

## Part 4 - Check everything is there
The repository should show: `.gitignore`, `Makefile`, `README.md`, and folders `data`, `docs`, `include`, `src`.
Open each folder and compare with the table:

| Folder | Number of files |
|---|---|
| `src` | 25 `.cpp` files |
| `include` | 17 `.h` files |
| `data` | 6 `.txt` files |
| `docs` | 3 `.md` files |

If a file is missing, open that folder -> Add file -> Upload files -> add only the missing file.

## Part 5 - Copy the link for submission
Open the repository home page -> copy the URL from Chrome's address bar
(for example `https://github.com/<your-username>/Hospital-Management-System`). Open the link in a private/incognito tab
to confirm it loads. If your repository is Private, give your teacher access (Settings -> Collaborators) or make it Public.

## Update the README credits
Open `README.md` on GitHub -> pencil icon -> replace the placeholders in section 13 with your names and UIDs and your repository link -> Commit.

## Faster alternative (if you are comfortable with commands): Termux
1. Install **Termux** from F-Droid. Run: `pkg install git unzip`
2. `termux-setup-storage`, then `cd ~/storage/downloads`
3. `unzip Hospital-Management-System.zip && cd Hospital-Management-System`
4. `git init && git add . && git commit -m "Hospital Management System"`
5. Create an empty repo on GitHub, create a Personal Access Token (Settings -> Developer settings -> Tokens), then:
   `git remote add origin https://github.com/<user>/<repo>.git`, `git branch -M main`, `git push -u origin main`
   (use your GitHub username and the token as the password).
Termux can also compile and run the program: `pkg install clang make` then `make CXX=clang++`.
