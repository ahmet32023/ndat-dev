#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>

// Pencere boyutları
const int PEN_BOY = 800;
const int PEN_EN = 600;

// Oyun alanı boyutları
const int ALAN_BOY = 800;
const int ALAN_EN = 600;

// Yılan özellikleri
const int yılan_boy = 20;
const int yılan_boy_ilk = 3;
std::vector<std::pair<int, int>> snake;
int yılanX = 1; // Yılanın X eksenindeki hareket yönü
int yılanY = 0; // Yılanın Y eksenindeki hareket yönü

// Yiyecek özellikleri
int yemX;
int yemY;

// Başlangıç menüsünde dönen küp özellikleri
float menuCubeRotation = 0.0f;

// Oyun durumu
bool oyun_basladı = false;
bool oyun_bitti = false;
bool baslangic_menu = true;
bool hazirlayanlar_acik = false;

// Skor
int skor = 0;






void baslangicMenuCiz() {
    // Küpü döndürme açısı
    menuCubeRotation += 3.0f;


    // "Oyuna Başla" seçeneği
    glColor3f(1.0, 1.0, 1.0); // Beyaz renk
    glRasterPos2f(ALAN_BOY / 2 - 80, ALAN_EN / 2 + 50);
    std::string oyunaBaslaMesaji = "Oyuna Basla (Space)";
    for (char const& c : oyunaBaslaMesaji) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }


    // "Hazırlayanlar" seçeneği
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


void yılanÇiz() {
    glColor3f(0.0, 1.0, 0.0); // Yeşil renk
    for (auto const& segment : snake) {
        glRectf(segment.first, segment.second, segment.first + yılan_boy, segment.second + yılan_boy);
    }
}

void yemÇiz() {
    glColor3f(1.0, 0.0, 0.0); // Kırmızı renk
    glRectf(yemX, yemY, yemX + yılan_boy, yemY + yılan_boy);
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
        // Yılanı, yiyeceği ve küpü çiz
        yılanÇiz();
        yemÇiz();
        skorCiz();
    }
    else {
        // Oyun bittiğinde ekrana bir mesaj yazdırabilirsiniz
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
    glTranslatef(ALAN_BOY / 2.0f, 300.0f, 1.0f); // Küpün başlangıç pozisyonunu ayarla
    glRotatef(menuCubeRotation, 100.0f, 100.0f, 1.0f); // Küpü döndür
    glutSolidCube(50.0); // 50 birim kenar uzunluğunda bir dolu küp çiz
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
        if (oyun_basladı && !oyun_bitti) {
            // Yılanı hareket ettir
            int newX = snake[0].first + yılanX * yılan_boy;
            int newY = snake[0].second + yılanY * yılan_boy;

            // Yılanın kendine çarpmasını kontrol et
            for (int i = 1; i < snake.size(); i++) {
                if (newX == snake[i].first && newY == snake[i].second) {
                    oyun_bitti = true;
                    break;
                }
            }

            if (!oyun_bitti) {
                // Yılanın ekranın dışına çıkmasını kontrol et
                if (newX < 0 || newX >= ALAN_BOY || newY < 0 || newY >= ALAN_EN) {
                    oyun_bitti = true;
                }
                else {
                    // Yılanın başını güncelle
                    snake.insert(snake.begin(), std::make_pair(newX, newY));

                    // Yiyecek yendi mi kontrol et
                    if (newX == yemX && newY == yemY) {
                        // Yılanın boyunu uzat
                        skor += 10;
                        // Yeni yiyecek konumu oluştur
                        yemX = (rand() % (ALAN_BOY / yılan_boy)) * yılan_boy;
                        yemY = (rand() % (ALAN_EN / yılan_boy)) * yılan_boy;
                    }
                    else {
                        // Yılanın sonundaki kuyruğu sil
                        snake.pop_back();
                    }
                }
            }
        }

        menuCubeRotation += 1.0f; // Başlangıç menüsünde dönen küpü her frame'de döndür
    }

    glutPostRedisplay();
    glutTimerFunc(100, baslat, 0);
}
void klavye(unsigned char key, int x, int y) {
    switch (key) {
    case ' ': // 'boşluk' tuşuna basıldığında oyunu başlat
        if (baslangic_menu) {
            baslangic_menu = false;
            oyun_basladı = true;
        }
        break;
    case 'h': // 'h' tuşuna basıldığında "Hazırlayanlar" menüsünü aç
        if (baslangic_menu) {
            hazirlayanlar_acik = !hazirlayanlar_acik;
        }
        break;
    case 27: // ESC tuşuna basıldığında çıkış yap
        exit(0);
        break;
    case 'w': // Yukarı ok tuşuna basıldığında yılanın yönünü yukarı değiştir
        if (yılanY != -1) {
            yılanX = 0;
            yılanY = 1;
        }
        break;
    case 's': // Aşağı ok tuşuna basıldığında yılanın yönünü aşağı değiştir
        if (yılanY != 1) {
            yılanX = 0;
            yılanY = -1;
        }
        break;
    case 'a': // Sol ok tuşuna basıldığında yılanın yönünü sola değiştir
        if (yılanX != 1) {
            yılanX = -1;
            yılanY = 0;
        }
        break;
    case 'd': // Sağ ok tuşuna basıldığında yılanın yönünü sağa değiştir
        if (yılanX != -1) {
            yılanX = 1;
            yılanY = 0;
        }
        break;
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(PEN_BOY, PEN_EN);
    glutCreateWindow("Yılan Oyunu");
    glutDisplayFunc(display);
    glutReshapeFunc(yenile);
    glutTimerFunc(0, baslat, 0);
    glutKeyboardFunc(klavye);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Yılanın başlangıç pozisyonunu belirle
    int initialSnakeX = ALAN_BOY / 2;
    int initialSnakeY = ALAN_EN / 2;
    for (int i = 0; i < yılan_boy_ilk; i++) {
        snake.push_back(std::make_pair(initialSnakeX - i * yılan_boy, initialSnakeY));
    }

    // İlk yiyecek konumunu belirle
    yemX = (rand() % (ALAN_BOY / yılan_boy)) * yılan_boy;
    yemY = (rand() % (ALAN_EN / yılan_boy)) * yılan_boy;

    glutMainLoop();

    return 0;
}
