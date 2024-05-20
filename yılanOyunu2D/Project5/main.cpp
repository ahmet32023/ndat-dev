#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>

// Pencere boyutlarý
const int PEN_BOY = 800;
const int PEN_EN = 600;

// Oyun alaný boyutlarý
const int ALAN_BOY = 800;
const int ALAN_EN = 600;

// Yýlan özellikleri
const int yýlan_boy = 20;
const int yýlan_boy_ilk = 3;
std::vector<std::pair<int, int>> snake;
int yýlanX = 1; // Yýlanýn X eksenindeki hareket yönü
int yýlanY = 0; // Yýlanýn Y eksenindeki hareket yönü

// Yiyecek özellikleri
int yemX;
int yemY;

// Baþlangýç menüsünde dönen küp özellikleri
float menuCubeRotation = 0.0f;

// Oyun durumu
bool oyun_basladý = false;
bool oyun_bitti = false;
bool baslangic_menu = true;
bool hazirlayanlar_acik = false;

// Skor
int skor = 0;






void baslangicMenuCiz() {
    // Küpü döndürme açýsý
    menuCubeRotation += 3.0f;


    // "Oyuna Baþla" seçeneði
    glColor3f(1.0, 1.0, 1.0); // Beyaz renk
    glRasterPos2f(ALAN_BOY / 2 - 80, ALAN_EN / 2 + 50);
    std::string oyunaBaslaMesaji = "Oyuna Basla (Space)";
    for (char const& c : oyunaBaslaMesaji) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }


    // "Hazýrlayanlar" seçeneði
    glRasterPos2f(ALAN_BOY / 2 - 80, ALAN_EN / 2 - 60);
    std::string hazirlayanlarMesaji = "Hazirlayanlar (H)";
    for (char const& c : hazirlayanlarMesaji) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    if (hazirlayanlar_acik) {
        glColor3f(1.0, 1.0, 1.0); // Beyaz renk
        glRasterPos2f(ALAN_BOY / 2 - 60, ALAN_EN / 2 - 100);
        std::string hazirlayanlarIsimler = "Fatih UNAL NO:200401011";
        for (char const& c : hazirlayanlarIsimler) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(ALAN_BOY / 2 - 60, ALAN_EN / 2 - 120);
        hazirlayanlarIsimler = "Ahmet SARI NO:210401033";
        for (char const& c : hazirlayanlarIsimler) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(ALAN_BOY / 2 - 60, ALAN_EN / 2 - 140);
        hazirlayanlarIsimler = "Oyku KURTGOZ NO:210401131";
        for (char const& c : hazirlayanlarIsimler) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(ALAN_BOY / 2 - 60, ALAN_EN / 2 - 160);
        hazirlayanlarIsimler = "Buse Nur YILMAZ NO:210401101";
        for (char const& c : hazirlayanlarIsimler) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }


}


void yýlanÇiz() {
    glColor3f(0.0, 1.0, 0.0); // Yeþil renk
    for (auto const& segment : snake) {
        glRectf(segment.first, segment.second, segment.first + yýlan_boy, segment.second + yýlan_boy);
    }
}

void yemÇiz() {
    glColor3f(1.0, 0.0, 0.0); // Kýrmýzý renk
    glRectf(yemX, yemY, yemX + yýlan_boy, yemY + yýlan_boy);
}

void skorCiz() {
    glColor3f(1.0, 1.0, 1.0); // Beyaz renk
    glRasterPos2f(10, 10);
    std::string skorMesaji = "Skor: " + std::to_string(skor);
    for (char const& c : skorMesaji) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (baslangic_menu) {
        baslangicMenuCiz();
    }

    else if (!oyun_bitti) {
        // Yýlaný, yiyeceði ve küpü çiz
        yýlanÇiz();
        yemÇiz();
        skorCiz();
    }
    else {
        // Oyun bittiðinde ekrana bir mesaj yazdýrabilirsiniz
        glColor3f(1.0, 1.0, 1.0); // Beyaz renk
        glRasterPos2f(ALAN_BOY / 2 - 60, ALAN_EN / 2);
        std::string gameOverMsg = "Game Over!";
        for (char const& c : gameOverMsg) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    // Küpü çiz
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0); // Mavi renk
    glTranslatef(ALAN_BOY / 2.0f, 300.0f, 1.0f); // Küpün baþlangýç pozisyonunu ayarla
    glRotatef(menuCubeRotation, 100.0f, 100.0f, 1.0f); // Küpü döndür
    glutSolidCube(50.0); // 50 birim kenar uzunluðunda bir dolu küp çiz
    glPopMatrix();

    glutSwapBuffers();
}


void yenile(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, ALAN_BOY, 0, ALAN_EN);
    glMatrixMode(GL_MODELVIEW);
}

void baslat(int value) {
    if (!baslangic_menu) {
        if (oyun_basladý && !oyun_bitti) {
            // Yýlaný hareket ettir
            int newX = snake[0].first + yýlanX * yýlan_boy;
            int newY = snake[0].second + yýlanY * yýlan_boy;

            // Yýlanýn kendine çarpmasýný kontrol et
            for (int i = 1; i < snake.size(); i++) {
                if (newX == snake[i].first && newY == snake[i].second) {
                    oyun_bitti = true;
                    break;
                }
            }

            if (!oyun_bitti) {
                // Yýlanýn ekranýn dýþýna çýkmasýný kontrol et
                if (newX < 0 || newX >= ALAN_BOY || newY < 0 || newY >= ALAN_EN) {
                    oyun_bitti = true;
                }
                else {
                    // Yýlanýn baþýný güncelle
                    snake.insert(snake.begin(), std::make_pair(newX, newY));

                    // Yiyecek yendi mi kontrol et
                    if (newX == yemX && newY == yemY) {
                        // Yýlanýn boyunu uzat
                        skor += 10;
                        // Yeni yiyecek konumu oluþtur
                        yemX = (rand() % (ALAN_BOY / yýlan_boy)) * yýlan_boy;
                        yemY = (rand() % (ALAN_EN / yýlan_boy)) * yýlan_boy;
                    }
                    else {
                        // Yýlanýn sonundaki kuyruðu sil
                        snake.pop_back();
                    }
                }
            }
        }

        menuCubeRotation += 1.0f; // Baþlangýç menüsünde dönen küpü her frame'de döndür
    }

    glutPostRedisplay();
    glutTimerFunc(100, baslat, 0);
}
void klavye(unsigned char key, int x, int y) {
    switch (key) {
    case ' ': // 'boþluk' tuþuna basýldýðýnda oyunu baþlat
        if (baslangic_menu) {
            baslangic_menu = false;
            oyun_basladý = true;
        }
        break;
    case 'h': // 'h' tuþuna basýldýðýnda "Hazýrlayanlar" menüsünü aç
        if (baslangic_menu) {
            hazirlayanlar_acik = !hazirlayanlar_acik;
        }
        break;
    case 27: // ESC tuþuna basýldýðýnda çýkýþ yap
        exit(0);
        break;
    case 'w': // Yukarý ok tuþuna basýldýðýnda yýlanýn yönünü yukarý deðiþtir
        if (yýlanY != -1) {
            yýlanX = 0;
            yýlanY = 1;
        }
        break;
    case 's': // Aþaðý ok tuþuna basýldýðýnda yýlanýn yönünü aþaðý deðiþtir
        if (yýlanY != 1) {
            yýlanX = 0;
            yýlanY = -1;
        }
        break;
    case 'a': // Sol ok tuþuna basýldýðýnda yýlanýn yönünü sola deðiþtir
        if (yýlanX != 1) {
            yýlanX = -1;
            yýlanY = 0;
        }
        break;
    case 'd': // Sað ok tuþuna basýldýðýnda yýlanýn yönünü saða deðiþtir
        if (yýlanX != -1) {
            yýlanX = 1;
            yýlanY = 0;
        }
        break;
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(PEN_BOY, PEN_EN);
    glutCreateWindow("Yýlan Oyunu");
    glutDisplayFunc(display);
    glutReshapeFunc(yenile);
    glutTimerFunc(0, baslat, 0);
    glutKeyboardFunc(klavye);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Yýlanýn baþlangýç pozisyonunu belirle
    int initialSnakeX = ALAN_BOY / 2;
    int initialSnakeY = ALAN_EN / 2;
    for (int i = 0; i < yýlan_boy_ilk; i++) {
        snake.push_back(std::make_pair(initialSnakeX - i * yýlan_boy, initialSnakeY));
    }

    // Ýlk yiyecek konumunu belirle
    yemX = (rand() % (ALAN_BOY / yýlan_boy)) * yýlan_boy;
    yemY = (rand() % (ALAN_EN / yýlan_boy)) * yýlan_boy;

    glutMainLoop();

    return 0;
}
