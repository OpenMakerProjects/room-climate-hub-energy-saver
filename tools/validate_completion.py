from pathlib import Path
import re,subprocess,xml.etree.ElementTree as ET
from decode_project_image import validate_png
root=Path(__file__).resolve().parents[1]
required=["README.md","LICENSE","docs/circuit-diagram.svg","docs/images/project-overview.png","platformio.ini"]
for item in required: assert (root/item).is_file(),item
svg=ET.parse(root/"docs/circuit-diagram.svg")
assert svg.getroot().tag=="{http://www.w3.org/2000/svg}svg"
text=(root/"docs/circuit-diagram.svg").read_text()
assert "<script" not in text and "http:" not in text.replace("http://www.w3.org/2000/svg","")
png=(root/"docs/images/project-overview.png").read_bytes()
print("PNG:",validate_png(png))
readme=(root/"README.md").read_text()
for target in re.findall(r"!\\[[^]]*\\]\\(([^)]+)\\)",readme):assert (root/target).exists(),target
assert "Permission is hereby granted" in (root/"LICENSE").read_text()
for path in root.rglob("*"):
 if not path.is_file() or ".git" in path.parts or ".pio" in path.parts or path.suffix==".png":continue
 data=path.read_text(errors="ignore")
 assert not re.search(r"gh[pousr]_[A-Za-z0-9]{30,}|AKIA[0-9A-Z]{16}|-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----",data),str(path)
subprocess.run(["python","-m","unittest","discover","-s","tests","-p","test_*.py"],check=True,cwd=root)
print("SVG, local image links, license and credential pattern scan passed")
