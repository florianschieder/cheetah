REM expects version passed as e.g. v1.42.1337

git commit --allow-empty -m "Version %1"
git tag %1
git push
git push origin --tags