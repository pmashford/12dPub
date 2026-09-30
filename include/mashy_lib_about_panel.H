//------------------------------------------------------------
// Programmer   Paul Mashford
// Date         27/9/26
// Description of Macro
// about_panel_fx.H -- a flashier drop-in replacement for 12dPub's
// mashy_lib_about_panel.H: same globals (about_macro_by / _for /
// _description / _changes, behind ABOUT_PANEL_DATA_INCLUDED) and the same
// calls (manage_about_panel(), manage_about_panel(Text)), so a macro
// switches by changing one #include. It defines the original's include
// guard, so the two can't both be compiled in.
//
// The Draw_Box shows the user's logo as a real solid (a cube with a
// notch cut from its near corner, which seen corner-on is the 2D logo of
// mashy_lib's create_logo()), lit (Lambert + ambient + a specular
// highlight), perspective-projected and depth-sorted (painter's
// algorithm), over a synthwave scene: gradient sky, twinkling stars, a
// striped sun and a scrolling perspective grid. The macro's title and
// author are drawn with a drop shadow.
//   mouse over the box   tilts and turns the logo (eased, with inertia)
//   click                spin burst
//   double click         next style: lit / lit + neon edges / wireframe /
//                        exploded (faces pushed out along their normals)
// Draw_Box has no timer, so a tiny PowerShell pump (written to %TEMP% at
// run time) posts WM_MOUSEMOVE to the box at 30 Hz while the About panel
// is the foreground window, and exits when the panel closes; its moves
// land at box (3|4, h-3) and are told apart from real ones. Without it the
// logo still animates on real mouse moves.
// Also fixed/added against the original: "Send Feedback" and "Copy Info"
// buttons (instead of the raw mailto text), a Changes tab when
// about_macro_changes is set, and the panel returns on CodeShutdown.
//------------------------------------------------------------

#ifndef mashy_lib_about_panel_included
#define mashy_lib_about_panel_included

#ifndef ABOUT_PANEL_DATA_INCLUDED
#define ABOUT_PANEL_DATA_INCLUDED
{
   Text about_macro_by = "Paul Mashford";
   Text about_macro_for = "ME";
   Text about_macro_description = "MY Generic about panel";
   Text about_macro_changes = "";
}
#endif

#define AFX_MAXF 16
#define AFX_STARS 70

{
   // logo mesh: a cube with a notch cut from its near corner, as quads
   // (4 corners each, anticlockwise seen from outside) with outward normals
   Real afx_qx[64];
   Real afx_qy[64];
   Real afx_qz[64];
   Real afx_nx[16];
   Real afx_ny[16];
   Real afx_nz[16];
   Integer afx_fr[16];
   Integer afx_fg[16];
   Integer afx_fb[16];
   Integer afx_nf = 0;
   // stars
   Real afx_sx[70];
   Real afx_sy[70];
   Real afx_sp[70];
   // view state
   Real afx_t0 = 0.;
   Real afx_last = 0.;
   Real afx_spin = 0.;
   Real afx_spin_vel = 0.;
   Real afx_yaw = 0.;
   Real afx_pitch = 0.;
   Real afx_tyaw = 0.;
   Real afx_tpitch = 0.;
   Integer afx_style = 1;
   Integer afx_w = 380;
   Integer afx_h = 180;
   Real afx_pump_seen = -99.;
}

Real afx_now()
{
   Real t = 0.;
   Get_time(t);
   return t;
}

Real afx_clamp(Real v, Real lo, Real hi)
{
   if (v < lo) return lo;
   if (v > hi) return hi;
   return v;
}

Integer afx_i(Real v)
{
   Real c = afx_clamp(v, 0., 255.);
   Integer i = Floor(c);
   return i;
}

//------------------------------------------------------------ mesh

// One axis-aligned square face. axis 1/2/3 = x/y/z plane at c, facing
// dir (+1/-1); a and b are the ranges of the other two axes in order
// (y,z for x planes, z,x for y planes, x,y for z planes).
void afx_face(Integer axis, Real c, Integer dir, Real a0, Real a1, Real b0, Real b1, Integer r, Integer g, Integer b)
{
   if (afx_nf >= AFX_MAXF) return;
   ++afx_nf;
   Integer k = (afx_nf - 1) * 4;
   Real ua[4];
   Real ub[4];
   // anticlockwise in (a, b) gives the +axis normal; reverse it for -axis
   ua[1] = a0; ub[1] = b0;
   ua[2] = a1; ub[2] = b0;
   ua[3] = a1; ub[3] = b1;
   ua[4] = a0; ub[4] = b1;
   for (Integer i = 1; i <= 4; ++i) {
      Integer j = i;
      if (dir < 0) j = 5 - i;
      if (axis == 1) {
         afx_qx[k + i] = c;
         afx_qy[k + i] = ua[j];
         afx_qz[k + i] = ub[j];
      } else if (axis == 2) {
         afx_qx[k + i] = ub[j];
         afx_qy[k + i] = c;
         afx_qz[k + i] = ua[j];
      } else {
         afx_qx[k + i] = ua[j];
         afx_qy[k + i] = ub[j];
         afx_qz[k + i] = c;
      }
   }
   afx_nx[afx_nf] = 0.;
   afx_ny[afx_nf] = 0.;
   afx_nz[afx_nf] = 0.;
   if (axis == 1) afx_nx[afx_nf] = dir;
   if (axis == 2) afx_ny[afx_nf] = dir;
   if (axis == 3) afx_nz[afx_nf] = dir;
   afx_fr[afx_nf] = r;
   afx_fg[afx_nf] = g;
   afx_fb[afx_nf] = b;
}

// The logo as a solid: a cube (-0.5..0.5) with the corner x 0..0.5,
// y 0..0.5, z -0.5..0 cut away. Seen from that corner (the view turns it
// to face the camera) it reads as the 2D logo: purple right (+x), blue top
// (+y), green front (-z), and the notch walls grey, grey and white.
void afx_build_mesh()
{
   afx_nf = 0;
   Real h = 0.5;
   // right face x = +0.5, facing +x (y,z ranges), minus the notch y 0..h, z -h..0
   afx_face(1, h, 1, -h, 0., -h, 0., 150, 80, 230);
   afx_face(1, h, 1, -h, 0., 0., h, 150, 80, 230);
   afx_face(1, h, 1, 0., h, 0., h, 150, 80, 230);
   // top face y = +0.5, facing +y (z,x ranges), minus the notch z -h..0, x 0..h
   afx_face(2, h, 1, -h, 0., -h, 0., 40, 130, 255);
   afx_face(2, h, 1, 0., h, -h, 0., 40, 130, 255);
   afx_face(2, h, 1, 0., h, 0., h, 40, 130, 255);
   // front face z = -0.5, facing -z (x,y ranges), minus the notch x 0..h, y 0..h
   afx_face(3, -h, -1, -h, 0., -h, 0., 40, 205, 115);
   afx_face(3, -h, -1, 0., h, -h, 0., 40, 205, 115);
   afx_face(3, -h, -1, -h, 0., 0., h, 40, 205, 115);
   // notch walls: x = 0 facing +x, y = 0 facing +y, z = 0 facing -z
   afx_face(1, 0., 1, 0., h, -h, 0., 185, 185, 200);
   afx_face(2, 0., 1, -h, 0., 0., h, 150, 150, 170);
   afx_face(3, 0., -1, 0., h, 0., h, 245, 245, 252);
   // back faces, darker
   afx_face(1, -h, -1, -h, h, -h, h, 90, 45, 140);
   afx_face(2, -h, -1, -h, h, -h, h, 25, 75, 150);
   afx_face(3, h, 1, -h, h, -h, h, 25, 120, 70);
   for (Integer s = 1; s <= AFX_STARS; ++s) {
      afx_sx[s] = Get_random_number_open();
      afx_sy[s] = Get_random_number_open();
      afx_sp[s] = Get_random_number_open() * 6.283;
   }
}

//------------------------------------------------------------ drawing

void afx_quad(Draw_Box box, Real x1, Real y1, Real x2, Real y2, Integer r, Integer g, Integer b)
{
   Real x[4];
   Real y[4];
   x[1] = x1; y[1] = y1;
   x[2] = x2; y[2] = y1;
   x[3] = x2; y[3] = y2;
   x[4] = x1; y[4] = y2;
   Set_colour(box, r, g, b);
   Set_fill_colour(box, r, g, b);
   Draw_polygon(box, 4, x, y);
}

void afx_line(Draw_Box box, Real x1, Real y1, Real x2, Real y2, Integer r, Integer g, Integer b)
{
   Set_colour(box, r, g, b);
   Move_to(box, x1, y1);
   Draw_to(box, x2, y2);
}

void afx_background(Draw_Box box, Real t, Real w, Real h, Real horizon)
{
   // sky gradient
   Integer bands = 22;
   for (Integer i = 0; i < bands; ++i) {
      Real f = i * 1. / bands;
      Real y1 = horizon + (h - horizon) * f;
      Real y2 = horizon + (h - horizon) * (i + 1.) / bands + 1.;
      afx_quad(box, 0., y1, w, y2, afx_i(90. - 80. * f), afx_i(25. - 12. * f), afx_i(110. - 70. * f));
   }
   // stars (twinkle, slight parallax with the view)
   for (Integer s = 1; s <= AFX_STARS; ++s) {
      Real sx = afx_sx[s] * w - afx_yaw * 6.;
      sx = sx - Floor(sx / w) * w;
      Real sy = horizon + 18. + afx_sy[s] * (h - horizon - 18.);
      Real b = 150. + 105. * Sin(t * 2.3 + afx_sp[s]);
      Real d = 1.;
      if (afx_sp[s] > 5.5) d = 2.;
      afx_quad(box, sx, sy, sx + d, sy + d, afx_i(b), afx_i(b), afx_i(b * 0.9 + 25.));
   }
   // striped sun on the horizon
   Real scx = w * 0.72;
   Real rad = h * 0.30;
   for (Integer k = 0; k < 18; ++k) {
      Real y1 = horizon + rad * k / 18.;
      Real y2 = horizon + rad * (k + 1.) / 18.;
      Real gap = 0.;
      if (k < 8) gap = (8. - k) * 0.55 + 0.8 * (1. + Sin(t * 1.5 + k));
      Real ym = (y1 + y2) / 2.;
      Real half = Sqrt(afx_clamp(rad * rad - (ym - horizon) * (ym - horizon), 0., rad * rad));
      Real f = k / 18.;
      if (y2 - gap > y1) afx_quad(box, scx - half, y1, scx + half, y2 - gap, 255, afx_i(60. + 150. * f), afx_i(140. - 90. * f));
   }
   // floor and scrolling grid
   afx_quad(box, 0., 0., w, horizon, 22, 6, 38);
   Real vx = w / 2.;
   Real scroll = t * 0.9;
   scroll = scroll - Floor(scroll);
   for (Integer j = 0; j < 12; ++j) {
      Real z = (j + 1. - scroll) * 0.6;
      Real y = horizon - horizon * 0.9 / z;
      if (y < 0. || y > horizon) continue;
      Real c = afx_clamp(255. * (1. - y / horizon), 60., 255.);
      afx_line(box, 0., y, w, y, afx_i(c), 40, afx_i(c * 0.8));
   }
   for (Integer v = -10; v <= 10; ++v) {
      Real xb = vx + v * w * 0.11 - afx_yaw * 30.;
      afx_line(box, vx + (xb - vx) * 0.05, horizon, xb, 0., 200, 50, 190);
   }
   afx_line(box, 0., horizon, w, horizon, 255, 120, 220);
}

// Transform, light, sort and draw the logo.
void afx_logo(Draw_Box box, Real t, Real w, Real h, Real horizon)
{
   // yaw 45 deg and pitch -35.26 deg point the notched corner (+x, +y, -z)
   // at the camera; sway, spin and the mouse turn it from there
   Real yaw = 0.7854 + afx_spin + afx_yaw + 0.30 * Sin(t * 0.8);
   Real pitch = -0.6155 + afx_pitch + 0.10 * Sin(t * 0.55);
   Real cy = Cos(yaw);
   Real sy = Sin(yaw);
   Real cp = Cos(pitch);
   Real sp = Sin(pitch);
   Real bob = 0.05 * Sin(t * 2.1);
   Real explode = 0.;
   if (afx_style == 4) explode = 0.12 * (1. + Sin(t * 1.7));
   Integer nv = afx_nf * 4;
   Real px[64];
   Real py[64];
   Real wx[64];
   Real wy[64];
   Real wz[64];
   Real cam = 3.2;
   Real focal = h * 1.2;
   Real ox = w * 0.48;
   Real oy = horizon + h * 0.13;
   for (Integer i = 1; i <= nv; ++i) {
      Integer f = (i - 1) / 4 + 1;
      Real x = afx_qx[i] + afx_nx[f] * explode;
      Real y = afx_qy[i] + afx_ny[f] * explode;
      Real z = afx_qz[i] + afx_nz[f] * explode;
      // yaw about y, then pitch about x
      Real x1 = x * cy + z * sy;
      Real z1 = -x * sy + z * cy;
      Real y1 = y * cp - z1 * sp;
      Real z2 = y * sp + z1 * cp;
      wx[i] = x1;
      wy[i] = y1 + bob;
      wz[i] = z2 + cam;
      px[i] = ox + focal * wx[i] / wz[i];
      py[i] = oy + focal * wy[i] / wz[i];
   }
   // shadow on the floor
   Real sw = w * 0.12 * (1. - bob);
   Real sxs[16];
   Real sys[16];
   for (Integer k = 1; k <= 16; ++k) {
      Real a = k * 6.2832 / 16.;
      sxs[k] = ox + sw * Cos(a);
      sys[k] = horizon * 0.55 + sw * 0.22 * Sin(a);
   }
   Set_colour(box, 12, 2, 20);
   Set_fill_colour(box, 12, 2, 20);
   Draw_polygon(box, 16, sxs, sys);

   // faces: cull back faces, key by depth, sort far to near
   Real key[16];
   Integer ord[16];
   Integer n = 0;
   for (Integer g = 1; g <= afx_nf; ++g) {
      Integer b0 = (g - 1) * 4;
      Real area = (px[b0 + 2] - px[b0 + 1]) * (py[b0 + 3] - py[b0 + 1]) - (px[b0 + 3] - px[b0 + 1]) * (py[b0 + 2] - py[b0 + 1]);
      if (afx_style != 3 && area > 0.) continue;
      ++n;
      ord[n] = g;
      key[n] = wz[b0 + 1] + wz[b0 + 2] + wz[b0 + 3] + wz[b0 + 4];
   }
   for (Integer sort_i = 2; sort_i <= n; ++sort_i) {
      Integer fo = ord[sort_i];
      Real ko = key[sort_i];
      Integer q = sort_i - 1;
      while (q >= 1) {
         if (key[q] >= ko) break;
         ord[q + 1] = ord[q];
         key[q + 1] = key[q];
         --q;
      }
      ord[q + 1] = fo;
      key[q + 1] = ko;
   }
   // direction to the light: upper left, on the camera's side (the camera
   // looks along +z, so -z is towards the viewer)
   Real lx = -0.48;
   Real ly = 0.60;
   Real lz = -0.64;
   for (Integer m = 1; m <= n; ++m) {
      Integer fc = ord[m];
      Integer c0 = (fc - 1) * 4;
      Real fx[4];
      Real fy[4];
      for (Integer e = 1; e <= 4; ++e) {
         fx[e] = px[c0 + e];
         fy[e] = py[c0 + e];
      }
      // the face normal, turned by the same rotation as the vertices
      Real mx = afx_nx[fc];
      Real my = afx_ny[fc];
      Real mz = afx_nz[fc];
      Real nx = mx * cy + mz * sy;
      Real nz1 = -mx * sy + mz * cy;
      Real ny = my * cp - nz1 * sp;
      Real nz = my * sp + nz1 * cp;
      Real lam = afx_clamp(nx * lx + ny * ly + nz * lz, 0., 1.);
      // Blinn half vector between the light and the view direction (0,0,-1)
      Real hx = lx;
      Real hy = ly;
      Real hz = lz - 1.;
      Real hl = Sqrt(hx * hx + hy * hy + hz * hz);
      Real spec = afx_clamp((nx * hx + ny * hy + nz * hz) / hl, 0., 1.);
      // spec^32: a tight sheen, so a face never whites out completely
      for (Integer sq = 1; sq <= 5; ++sq) spec = spec * spec;
      Real shade = 0.40 + 0.75 * lam;
      Integer r = afx_i(afx_fr[fc] * shade + 110. * spec);
      Integer gr = afx_i(afx_fg[fc] * shade + 110. * spec);
      Integer b = afx_i(afx_fb[fc] * shade + 110. * spec);
      if (afx_style == 3) {
         Real hue = t * 2. + fc * 0.4;
         Integer er = afx_i(128. + 127. * Sin(hue));
         Integer eg = afx_i(128. + 127. * Sin(hue + 2.1));
         Integer eb = afx_i(128. + 127. * Sin(hue + 4.2));
         for (Integer e1 = 1; e1 <= 4; ++e1) {
            Integer e2 = e1 % 4 + 1;
            afx_line(box, fx[e1], fy[e1], fx[e2], fy[e2], er, eg, eb);
         }
         continue;
      }
      Set_colour(box, r, gr, b);
      Set_fill_colour(box, r, gr, b);
      Draw_polygon(box, 4, fx, fy);
      if (afx_style == 2) {
         for (Integer e3 = 1; e3 <= 4; ++e3) {
            Integer e4 = e3 % 4 + 1;
            afx_line(box, fx[e3], fy[e3], fx[e4], fy[e4], 255, 120, 230);
         }
      }
   }
}

void afx_text(Draw_Box box, Real x, Real y, Real ht, Text t, Integer r, Integer g, Integer b)
{
   Set_text_colour(box, 20, 0, 30);
   Draw_text(box, x + 1.5, y - 1.5, ht, 0., t);
   Set_text_colour(box, r, g, b);
   Draw_text(box, x, y, ht, 0., t);
}

void afx_frame(Draw_Box box)
{
   Real now = afx_now();
   Real dt = afx_clamp(now - afx_last, 0., 0.1);
   afx_last = now;
   Real t = now - afx_t0;
   // ease towards the cursor, spin with inertia
   afx_yaw += (afx_tyaw - afx_yaw) * afx_clamp(dt * 6., 0., 1.);
   afx_pitch += (afx_tpitch - afx_pitch) * afx_clamp(dt * 6., 0., 1.);
   Real turn = 6.28318530718;
   Real home = Floor(afx_spin / turn + 0.5) * turn;
   afx_spin_vel += (home - afx_spin) * 6. * dt;
   afx_spin_vel -= afx_spin_vel * afx_clamp(dt * 2.2, 0., 1.);
   afx_spin += afx_spin_vel * dt;
   Real w = afx_w;
   Real h = afx_h;
   Real horizon = h * 0.40;
   Start_batch_draw(box);
   Clear(box, 10, 6, 30);
   afx_background(box, t, w, h, horizon);
   afx_logo(box, t, w, h, horizon);
   Set_bk_mode(box, 1);  // transparent text background (the default is opaque)
   Set_text_font(box, "Arial");
   Set_text_weight(box, 700);
   afx_text(box, 10., h - 8., 15., about_macro_for, 255, 255, 255);
   Set_text_weight(box, 400);
   afx_text(box, 10., h - 27., 11., "by " + about_macro_by, 255, 200, 245);
   Text st = "lit";
   if (afx_style == 2) st = "neon";
   if (afx_style == 3) st = "wireframe";
   if (afx_style == 4) st = "exploded";
   afx_text(box, w - 128., 16., 9., "double click: " + st, 200, 170, 230);
   End_batch_draw(box);
}

//------------------------------------------------------------ pump

Integer afx_write(File f, Text line)
{
   return File_write_line(f, line);
}

// Write the pump script to %TEMP% and start it through cmd.exe
// (Create_process can't start powershell.exe directly).
Integer afx_start_pump(Text title, Integer w, Integer h)
{
   Text tmp = "";
   Get_temporary_directory(tmp);
   Text sep = "\\";
   if (Get_subtext(tmp, Text_length(tmp), Text_length(tmp)) == "\\") sep = "";
   Text ps1 = tmp + sep + "about_panel_fx_pump.ps1";
   if (File_exists(ps1)) File_delete(ps1);
   File f;
   if (File_open(ps1, "w", "", f) != 0) return 1;
   afx_write(f, "param([string]$Title, [int]$W, [int]$H, [int]$Hz = 30)");
   afx_write(f, "Add-Type -Namespace AboutFx -Name Win -MemberDefinition @'");
   afx_write(f, "public delegate bool EnumProc(System.IntPtr h, System.IntPtr l);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern bool EnumWindows(EnumProc p, System.IntPtr l);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern bool EnumChildWindows(System.IntPtr p, EnumProc c, System.IntPtr l);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\", CharSet = System.Runtime.InteropServices.CharSet.Unicode)] public static extern int GetWindowText(System.IntPtr h, System.Text.StringBuilder s, int n);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern bool IsWindowVisible(System.IntPtr h);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern bool IsWindow(System.IntPtr h);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern bool GetClientRect(System.IntPtr h, out RECT r);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern bool PostMessage(System.IntPtr h, uint m, System.IntPtr w, System.IntPtr l);");
   afx_write(f, "[System.Runtime.InteropServices.DllImport(\"user32.dll\")] public static extern System.IntPtr GetForegroundWindow();");
   afx_write(f, "[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }");
   afx_write(f, "'@");
   afx_write(f, "$script:panel = [IntPtr]::Zero");
   afx_write(f, "$t0 = Get-Date");
   afx_write(f, "while ($script:panel -eq [IntPtr]::Zero -and ((Get-Date) - $t0).TotalSeconds -lt 10) {");
   afx_write(f, "  [AboutFx.Win]::EnumWindows({ param($hw, $lp)");
   afx_write(f, "    if ([AboutFx.Win]::IsWindowVisible($hw)) { $sb = New-Object System.Text.StringBuilder 512; [void][AboutFx.Win]::GetWindowText($hw, $sb, 512); if ($sb.ToString() -eq $Title) { $script:panel = $hw; return $false } }");
   afx_write(f, "    return $true }, [IntPtr]::Zero) | Out-Null");
   afx_write(f, "  if ($script:panel -eq [IntPtr]::Zero) { Start-Sleep -Milliseconds 200 } }");
   afx_write(f, "if ($script:panel -eq [IntPtr]::Zero) { exit 1 }");
   afx_write(f, "$script:box = [IntPtr]::Zero");
   afx_write(f, "[AboutFx.Win]::EnumChildWindows($script:panel, { param($hw, $lp)");
   afx_write(f, "  $r = New-Object AboutFx.Win+RECT; [void][AboutFx.Win]::GetClientRect($hw, [ref]$r)");
   afx_write(f, "  if (($r.R - $r.L) -eq $W -and ($r.B - $r.T) -eq $H -and $script:box -eq [IntPtr]::Zero) { $script:box = $hw }");
   afx_write(f, "  return $true }, [IntPtr]::Zero) | Out-Null");
   afx_write(f, "if ($script:box -eq [IntPtr]::Zero) { exit 2 }");
   afx_write(f, "$n = 0");
   afx_write(f, "while ([AboutFx.Win]::IsWindow($script:panel)) {");
   afx_write(f, "  if ([AboutFx.Win]::GetForegroundWindow() -eq $script:panel) { [void][AboutFx.Win]::PostMessage($script:box, 0x200, [IntPtr]::Zero, [IntPtr](3 + ($n % 2) + (3 -shl 16))); $n++ }");
   afx_write(f, "  Start-Sleep -Milliseconds ([int](1000 / $Hz)) }");
   File_close(f);
   Text ps = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
   Text args = "cmd.exe /c \"\"" + ps + "\" -NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File \"" + ps1 + "\" -Title \""
      + title + "\" -W " + To_text(w) + " -H " + To_text(h) + "\"";
   // 134217728 = CREATE_NO_WINDOW; don't wait
   return Create_process("C:\\Windows\\System32\\cmd.exe", args, tmp, 134217728, 0, 0);
}

//------------------------------------------------------------ panel

// The running macro's path as Explorer needs it: backslashes, and a leading
// $NAME\... resolved from the environment (NAME, then NAME_4D), as
// mashy_lib_text_functions.H's replace_env_path does.
Text afx_macro_path()
{
   Text src = Get_macro_name();
   Text expanded = "";
   for (Integer i = 1; i <= Text_length(src); ++i) {
      Text ch = Get_subtext(src, i, i);
      if (ch == "/") ch = "\\";
      expanded += ch;
   }
   if (Get_subtext(expanded, 1, 1) == "$") {
      Integer slash = Find_text(expanded, "\\");
      Integer end = Text_length(expanded) + 1;
      if (slash > 0) end = slash;
      Text name = Text_upper(Get_subtext(expanded, 2, end - 1));
      Text root = Getenv(name);
      if (root == "") root = Getenv(name + "_4D");
      if (root != "") expanded = root + Get_subtext(expanded, end, Text_length(expanded));
   }
   return expanded;
}

// Explorer with the .4do selected, started through cmd's "start" (as the
// original's run_system_command_no_wait does; Create_process on
// explorer.exe itself didn't open anything).
Integer afx_open_location()
{
   Text path = afx_macro_path();
   return Create_process("C:\\Windows\\System32\\cmd.exe", "cmd.exe /c start \"\" explorer.exe /select,\"" + path + "\"", "", 134217728,
      0, 0);
}

Text afx_url(Text t)
{
   Text out = "";
   for (Integer i = 1; i <= Text_length(t); ++i) {
      Text ch = Get_subtext(t, i, i);
      if (ch == "\n") out += "%0A";
      else if (ch == " ") out += "%20";
      else if (ch == "&") out += "%26";
      else if (ch == "#") out += "%23";
      else if (ch == "\\") out += "%5C";
      else if (ch == "\"") out += "%22";
      else out += ch;
   }
   return out;
}

Text afx_12d_info()
{
   Text user = "";
   Text project = "";
   Text folder = "";
   Get_user_name(user);
   Get_project_name(project);
   Get_project_folder(folder);
   Text t = "12d Version: " + Get_program_full_title_name() + "\n";
   t += "12d Patch: " + Get_program_patch_version_name() + "\n";
   t += "12d exe: " + Get_program() + "\n";
   t += "\nMacro: " + afx_macro_path() + "\n";
   t += "User: " + user + "\n";
   t += "Project: " + project + "\n";
   t += "Project folder: " + folder + "\n";
   return t;
}

Text afx_build_info()
{
   Text t = "Compiler version: " + To_text(VERSION_4D) + "\n";
#ifdef QUOTED_CC4D_VERSION_DATA
   t += "Compiler info: " + QUOTED_CC4D_VERSION_DATA + "\n";
#endif
   t += "Compiled: " + __DATE__ + " " + __TIME__ + "\n";
   return t;
}

Text_Edit_Box afx_tab_text(Text text, Message_Box message, Vertical_Group tab)
{
   Text_Edit_Box box = Create_text_edit_box("", message, 7);
   Set_read_only(box, 1);
   Set_word_wrap(box, 1);
   Set_data(box, text);
   Append(box, tab);
   return box;
}

Integer manage_about_panel(Text description)
{
   Text title = about_macro_for + " | About Macro";
   Panel panel = Create_panel(title);
   Message_Box message_box = Create_message_box("");
   Vertical_Group vg = Create_vertical_group(0);
   Draw_Box draw_box = Create_draw_box(afx_w, afx_h, 0);
   Horizontal_Group buttons = Create_button_group();
   Button open_button = Create_button("Open Macro Location", "afx open");
   Button copy_button = Create_button("Copy Info", "afx copy");
   Button mail_button = Create_button("Send Feedback", "afx mail");
   Append(open_button, buttons);
   Append(copy_button, buttons);
   Append(mail_button, buttons);

   Text info = "For: " + about_macro_for + "\nBy: " + about_macro_by + "\n";
   Text build = afx_build_info();
   Text info12d = afx_12d_info();

   Tab_Box tabs = Create_tab_box();
   Vertical_Group t1 = Create_vertical_group(0);
   Vertical_Group t2 = Create_vertical_group(0);
   Vertical_Group t3 = Create_vertical_group(0);
   Vertical_Group t4 = Create_vertical_group(0);
   Vertical_Group t5 = Create_vertical_group(0);
   afx_tab_text(description, message_box, t1);
   afx_tab_text(info, message_box, t2);
   afx_tab_text(build, message_box, t3);
   afx_tab_text(info12d, message_box, t4);
   Append(t1, "Description", tabs);
   Append(t2, "Info", tabs);
   Append(t3, "Build", tabs);
   Append(t4, "12d Info", tabs);
   if (about_macro_changes != "") {
      afx_tab_text(about_macro_changes, message_box, t5);
      Append(t5, "Changes", tabs);
   }

   Append(draw_box, vg);
   Append(buttons, vg);
   Append(tabs, vg);
   Append(message_box, vg);
   Append(vg, panel);
   Show_widget(panel);

   Integer bw = 0;
   Integer bh = 0;
   Get_size(draw_box, bw, bh);
   if (bw > 0 && bh > 0) {
      afx_w = bw;
      afx_h = bh;
   }
   afx_build_mesh();
   afx_t0 = afx_now();
   afx_last = afx_t0;
   afx_frame(draw_box);
   Integer pump = afx_start_pump(title, afx_w, afx_h);
   Text all = "Macro: " + about_macro_for + "\n" + info + "\n" + build + "\n" + info12d;

   while (1) {
      Integer id = 0;
      Text cmd = "";
      Text msg = "";
      Wait_on_widgets(id, cmd, msg);
      if (cmd == "Panel Quit") return 0;
      if (cmd == "CodeShutdown") {
#if VERSION_4D > 1400
         Set_exit_code(cmd);
#endif
         return 0;
      }
      if (id == Get_id(draw_box)) {
         if (cmd == "mouse_move") {
            Real mx = 0.;
            Real my = 0.;
            Dynamic_Text parts;
            From_text(msg, parts);
            Text a = "";
            Text b = "";
            Get_item(parts, 1, a);
            Get_item(parts, 2, b);
            From_text(a, mx);
            From_text(b, my);
            Null(parts);
            // the pump's moves land at (3|4, h-3); real ones steer the logo
            Integer pumped = 0;
            if (mx <= 5.) {
               if (my >= afx_h - 5.) pumped = 1;
            }
            if (pumped) afx_pump_seen = afx_now();
            else if (mx >= 0. && mx <= afx_w && my >= 0. && my <= afx_h) {
               afx_tyaw = (mx / afx_w - 0.5) * 1.4;
               afx_tpitch = (0.5 - my / afx_h) * 0.8;
            }
            afx_frame(draw_box);
         } else if (cmd == "click_lb") {
            afx_spin_vel += 14.;
            afx_frame(draw_box);
         } else if (cmd == "double_click_lb") {
            afx_style = afx_style % 4 + 1;
            afx_frame(draw_box);
         }
         continue;
      }
      if (cmd == "afx open") {
         Integer ro = afx_open_location();
         Set_data(message_box, "opening " + afx_macro_path() + " (" + To_text(ro) + ")");
      } else if (cmd == "afx copy") {
         Set_clipboard_text(all);
         Set_data(message_box, "macro, build and 12d info copied to the clipboard");
      } else if (cmd == "afx mail") {
         Text url = "mailto:p.mashford@gmail.com?subject=" + afx_url("Macro: " + about_macro_for) + "&body="
            + afx_url("Comments:\n\n\n" + all);
         Shell_execute(mail_button, "open", url, "", "", 1);
         Set_data(message_box, "opening your email program...");
      }
   }
   return 0;
}

Integer manage_about_panel()
{
   return manage_about_panel(about_macro_description);
}

#endif
