import math
import sys

class Vec3:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __sub__(self, other):
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __add__(self, other):
        return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __mul__(self, other):
        if isinstance(other, Vec3):
            return self.x * other.x + self.y * other.y + self.z * other.z
        else:
            return Vec3(self.x * other, self.y * other, self.z * other)
    
    def __truediv__(self, other):
        if isinstance(other, Vec3):
            return Vec3(self.x / other.x, self.y / other.y, self.z / other.z)
        else:
            return Vec3(self.x / other, self.y / other, self.z / other)

    def __xor__(self, other):
        # Cross product
        return Vec3(
            self.y * other.z - self.z * other.y,
            self.z * other.x - self.x * other.z,
            self.x * other.y - self.y * other.x
        )

    def __repr__(self):
        return f"Vec3({self.x}, {self.y}, {self.z})"

    def as_list(self):
        return [self.x, self.y, self.z]

class Vtx:
    def __init__(self, line):
        ls = [ tok.replace('{', '').replace('}', '').strip() for tok in line.split(',') ]
        ls = [ int(tok) for tok in ls if tok ]
        self.pos = Vec3(ls[0], ls[1], ls[2])
        self.uv = [ ls[4], ls[5] ]
        self.color = [ ls[6], ls[7], ls[8], ls[9] ]

    def __repr__(self):
        return f"Vtx(pos={self.pos}, uv={self.uv}, color={self.color})"

    def __str__(self):
        return f"{{ {self.pos.x}, {self.pos.y}, {self.pos.z}, 0, {self.uv[0]}, {self.uv[1]}, {self.color[0]}, {self.color[1]}, {self.color[2]}, {self.color[3]} }},"

def load_and_fix(model_path):
    lines = []
    
    with open(model_path, "r") as f_model:
        tri_vtx = []
        first_vtx = False
        keep_loading = False
        while True:
            line = f_model.readline()
            if not line:
                break

            if 'Vtx' in line:
                assert not keep_loading
                keep_loading = True
                first_vtx = True
                lines.append(line)
            elif '};' in line:
                print(tri_vtx)
                assert len(tri_vtx) == 0
                keep_loading = False
                lines.append(line)
            elif keep_loading:
                vtx = Vtx(line)
                if first_vtx and (vtx.color[0] != 0 or vtx.color[1] != 0 or vtx.color[2] != 0):
                    lines.append(line)
                    continue

                first_vtx = False
                tri_vtx.append(Vtx(line))
                if len(tri_vtx) == 3:
                    # Calculate normal for the triangle
                    # Conveniently enough ancient importer did not attempt to optimize the vbo so this is valid
                    v0 = tri_vtx[0].pos
                    v1 = tri_vtx[1].pos
                    v2 = tri_vtx[2].pos

                    edge1 = v1 - v0
                    edge2 = v2 - v0
                    normal = edge1 ^ edge2

                    for vtx in tri_vtx:
                        ndot = normal * normal
                        if ndot == 0:
                            n = Vec3(0, 0, 0)
                        else:
                            n = normal / math.sqrt(normal * normal)

                        r = int(n.x * 127)
                        g = int(n.y * 127)
                        b = int(n.z * 127)
                        vtx.color[0] = r
                        vtx.color[1] = g
                        vtx.color[2] = b

                        lines.append(f"{vtx}\n")

                    tri_vtx = []
            else:
                lines.append(line)

    return lines

if '__main__' in __name__:
    model_path = sys.argv[1]
    model_patched_path = load_and_fix(model_path)
    with open(model_path, "w") as f_out:
        for line in model_patched_path:
            f_out.write(line)
