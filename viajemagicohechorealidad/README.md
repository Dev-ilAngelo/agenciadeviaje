# Magical Travel Premium ✈️

Una plataforma de gestión de viajes de alta gama construida con un enfoque **Domain-Driven Design (DDD)** y estética de nivel corporativo ($50k USD UI). Este proyecto moderniza un sistema legado en C, transformándolo en una aplicación Next.js escalable, segura y visualmente impactante.

## 🚀 Características Clave

- **Premium UX/UI**: Diseño vanguardista con Glassmorphism, animaciones físicas (Framer Motion) y Toggle de Modo Oscuro persistente.
- **Smart Chat Assistant**: Un asistente de reserva guiado por estados que valida datos en tiempo real y calcula precios dinámicamente.
- **Arquitectura Robusta**:
  - **Dominio**: Lógica de negocio centralizada (validación de DNI, reglas de recargo por edad/pago).
  - **API Layer**: Validación estricta con Zod y manejo de cuotas por destino.
- **Dashboard Administrativo**: Panel de control senior con analíticas en tiempo real y gestión de pasajeros.
- **Vercel Ready**: Optimizado para despliegue instantáneo con Server Components.

## 🛠️ Stack Tecnológico

- **Framework**: Next.js 15+ (App Router)
- **Lenguaje**: TypeScript (Strict Mode)
- **Estilos**: TailwindCSS
- **Animaciones**: Framer Motion
- **Iconos**: Lucide React
- **Validación**: Zod
- **Temas**: Next-Themes

## 📋 Lógica de Negocio (Mapeada de C)

- **Validación de DNI**: Rango estricto 5M - 60M (Legacy business rule).
- **Recargos**:
  - Menores de 5 años: +$2000 (Calculado en backend).
  - Pago con Tarjeta: +5% sobre el total acumulado.
- **Cuotas**: Límite estricto de 60 pasajeros por destino (BRA, MDQ, MZA, BRC).

## 💻 Instalación Local

```bash
# 1. Clonar el repositorio
git clone <url-del-repo>

# 2. Instalar dependencias
npm install

# 3. Iniciar modo desarrollo
npm run dev
```

## 📄 Licencia

Este proyecto fue desarrollado como parte de un portfolio profesional de alto nivel, demostrando habilidades de Arquitectura de Software e Ingeniería de Producto.
